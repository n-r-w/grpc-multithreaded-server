#include "srv_sql.h"

#include "../hrs_factory.h"
#include <utils/sl_utils.h>
#include <api/src/data_converter.h>
#include <sql_lib/plugins/psql/psql_impl.h>

namespace hrs
{
void SqlRequestProcessor::handleRequest()
{
    sql::Error error;
    auto connection
        = HrsServiceFactory::instance()->sqlConnectionPool()->getConnection("127.0.0.1", 5432, "ML829MP1", "postgres", "1", "", error);

    std::unique_ptr<sql::PsqlQuery> query;

    if (error.isOk()) {
        query = std::make_unique<sql::PsqlQuery>(connection);
        error = query->exec(request()->sql());
    }

    if (error.isError()) {
        reply()->set_error_text(error.text());
        reply()->set_error_code(error.code());

    } else {
        auto container = new ProtoShared::DataContainer;
        auto writer = api::DataContainerWrapper::createWriter(container, false);

        const int dataset_id = 1;

        writer->initDataset(dataset_id, query->rowCount(), query->columnCount());

        for (size_t row = 0; row < query->rowCount(); row++) {
            for (size_t col = 0; col < query->columnCount(); col++) {
                writer->datasetWriter(dataset_id)->setValue(row, col, query->toString(row, col));
            }
        }

        reply()->set_allocated_data(container);
    }
}
} // namespace hrs
