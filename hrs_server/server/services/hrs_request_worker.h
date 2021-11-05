#pragma once
#include <server_lib/sl_request_worker.h>

namespace hrs
{
//! Обработчик очереди, работает в отдельном потоке и отвечает за создание обработчиков запросов
class HrsRequestWorker : public sl::RequestWorker
{
public:
    HrsRequestWorker(
        //! Очередь
        grpc::ServerCompletionQueue* queue,
        //! Проверка пользователей
        sl::UserValidator* user_validator);

protected:
    //! Фабрика по созданию обработчиков запросов
    std::vector<sl::RequestProcessorBase*> createRequestProcessors(const std::string& service_key) const override;
    //! Извлечь из запроса логин и пароль
    UservValidationResult extractUserValidationInfo(const grpc::Service* service, const grpc::ServerContext* context, std::string& login,
                                                    std::string& password) const override;
};

} // namespace hrs
