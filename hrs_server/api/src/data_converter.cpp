#include "data_converter.h"
#include <iomanip>
#include <sstream>

using ProtoShared::PacketDataset;

namespace api
{
static std::string serializeTimePoint(const std::chrono::system_clock::time_point& time, const std::string& format)
{
    std::time_t tt = std::chrono::system_clock::to_time_t(time);
    std::tm tm = *std::gmtime(&tt); //GMT (UTC)
    std::stringstream ss;
    ss << std::put_time(&tm, format.c_str());
    return ss.str();
}

DatasetWrapper::DatasetWrapper(const ProtoShared::PacketDataset& dataset)
    : DatasetWrapper(&dataset)
{
}

DatasetWrapper::DatasetWrapper(const ProtoShared::PacketDataset* dataset)
    : _packet_data_ro(dataset)
{
    assert(dataset != nullptr);
}

DatasetWrapper::DatasetWrapper(ProtoShared::PacketDataset* dataset)
    : _packet_data_w(dataset)
    , _packet_data_ro(dataset)
{
    assert(dataset != nullptr);
}

DatasetWrapper::DatasetWrapper(ProtoShared::PacketDataset& dataset)
    : DatasetWrapper(&dataset)
{
}

DatasetWrapper::DatasetWrapper(const ProtoShared::FlatDataset& dataset)
    : DatasetWrapper(&dataset)
{
}

DatasetWrapper::DatasetWrapper(const ProtoShared::FlatDataset* dataset)
    : _flat_data_ro(dataset)
{
    assert(dataset != nullptr);
}

DatasetWrapper::DatasetWrapper(ProtoShared::FlatDataset* dataset)
    : _flat_data_w(dataset)
    , _flat_data_ro(dataset)
{
    assert(dataset != nullptr);    
}

DatasetWrapper::DatasetWrapper(ProtoShared::FlatDataset& dataset)
    : DatasetWrapper(&dataset)
{
}

size_t DatasetWrapper::rowCount() const
{
    return _packet_data_ro ? _packet_data_ro->row_count() : _flat_data_ro->rows_size();
}

size_t DatasetWrapper::columnCount() const
{
    return _packet_data_ro ? _packet_data_ro->column_count() : _flat_data_ro->column_count();
}

bool DatasetWrapper::hasRow(size_t row) const
{
    return _packet_data_ro ? _packet_data_ro->rows().contains(row) : _flat_data_ro->rows_size() > row;
}

void DatasetWrapper::grow(size_t row_count, size_t column_count)
{
    if (_packet_data_ro) {
        assert(_packet_data_w != nullptr);

        if (row_count > _packet_data_w->row_count())
            _packet_data_w->set_row_count(row_count);

        if (column_count > _packet_data_w->column_count())
            _packet_data_w->set_column_count(column_count);

    } else {
        assert(_flat_data_ro != nullptr && row_count < _flat_data_ro->rows_size() && column_count < _flat_data_ro->column_count());
    }
}

bool DatasetWrapper::hasValue(size_t row, size_t column) const
{
    return rawValue(row, column, false) != nullptr;
}

DataType DatasetWrapper::type(size_t row, size_t column) const
{
    return ValueWrapper::type(rawValue(row, column, true));
}

const std::string* DatasetWrapper::toString(size_t row, size_t column) const
{
    return ValueWrapper::toString(rawValue(row, column, true));
}

int64_t DatasetWrapper::toInt(size_t row, size_t column) const
{
    return ValueWrapper::toInt(rawValue(row, column, true));
}

double DatasetWrapper::toDouble(size_t row, size_t column) const
{
    return ValueWrapper::toDouble(rawValue(row, column, true));
}

bool DatasetWrapper::toBool(size_t row, size_t column) const
{
    return ValueWrapper::toBool(rawValue(row, column, true));
}

const std::string* DatasetWrapper::toBytes(size_t row, size_t column) const
{
    return ValueWrapper::toBytes(rawValue(row, column, true));
}

uint64_t DatasetWrapper::toDateTime(size_t row, size_t column) const
{
    return ValueWrapper::toDateTime(rawValue(row, column, true));
}

uint32_t DatasetWrapper::toTime(size_t row, size_t column) const
{
    return ValueWrapper::toTime(rawValue(row, column, true));
}

bool DatasetWrapper::isNull(size_t row, size_t column) const
{
    return ValueWrapper::isNull(rawValue(row, column, true));
}

std::string DatasetWrapper::convertToString(size_t row, size_t column) const
{
    if (!hasValue(row, column))
        return {};
    return ValueWrapper::convertToString(rawValue(row, column, true));
}

void DatasetWrapper::setValue(size_t row, size_t column, int64_t v)
{
    ValueWrapper::setValue(rawValue(row, column, true), v);
}

void DatasetWrapper::setValue(size_t row, size_t column, double v)
{
    ValueWrapper::setValue(rawValue(row, column, true), v);
}

void DatasetWrapper::setValue(size_t row, size_t column, bool v)
{
    ValueWrapper::setValue(rawValue(row, column, true), v);
}

void DatasetWrapper::setValue(size_t row, size_t column, const std::string& v)
{
    ValueWrapper::setValue(rawValue(row, column, true), v);
}

void DatasetWrapper::setValue(size_t row, size_t column, const char* v)
{
    ValueWrapper::setValue(rawValue(row, column, true), std::string(v));
}

void DatasetWrapper::setValue(size_t row, size_t column, const std::vector<char>& v)
{
    ValueWrapper::setValue(rawValue(row, column, true), v);
}

void DatasetWrapper::setDateTime(size_t row, size_t column, uint64_t v)
{
    ValueWrapper::setDateTime(rawValue(row, column, true), v);
}

void DatasetWrapper::setTime(size_t row, size_t column, uint32_t v)
{
    ValueWrapper::setTime(rawValue(row, column, true), v);
}

void DatasetWrapper::setNull(size_t row, size_t column)
{
    ValueWrapper::setNull(rawValue(row, column, true));
}

void DatasetWrapper::clearValue(size_t row, size_t column)
{
    assert(row < rowCount());
    assert(column < columnCount());

    auto rows = _packet_data_w->mutable_rows();
    auto r = rows->find(row);
    if (r == rows->end())
        return;

    r->second.mutable_data()->erase(column);
    if (r->second.mutable_data()->empty())
        _packet_data_w->mutable_rows()->erase(row);
}

ProtoShared::PacketDataset* DatasetWrapper::data() const
{
    return _packet_data_w;
}

const ProtoShared::PacketDataset* DatasetWrapper::constData() const
{
    return _packet_data_ro;
}

const ProtoShared::Value* DatasetWrapper::rawValue(size_t row, size_t column, bool check) const
{
    if (check) {
        assert(row < rowCount());
        assert(column < columnCount());

    } else {
        if (row >= rowCount())
            return nullptr;
        if (column >= columnCount())
            return nullptr;
    }

    if (_packet_data_ro != nullptr) {
        auto data = _packet_data_ro->rows().find(row);
        if (data == _packet_data_ro->rows().cend())
            return nullptr;

        auto i = data->second.data().find(column);
        if (i == data->second.data().cend())
            return nullptr;

        return &i->second;

    } else {
        return &_flat_data_ro->rows(row).data(column);
    }
}

ProtoShared::Value* DatasetWrapper::rawValue(size_t row, size_t column, bool create)
{
    if (_packet_data_ro != nullptr) {
        assert(_packet_data_w != nullptr);

        if (!create && hasValue(row, column))
            return nullptr;

        grow(row + 1, column + 1);

        auto rows = _packet_data_w->mutable_rows();
        auto r = rows->find(row);

        if (r == rows->end()) {
            (*rows)[row] = PacketDataset::Row();
            r = rows->find(row);
            assert(r != rows->end());
        }

        auto c = r->second.mutable_data()->find(column);
        if (c == r->second.mutable_data()->end()) {
            (*r->second.mutable_data())[column] = ProtoShared::Value();
            c = r->second.mutable_data()->find(column);
            assert(c != r->second.mutable_data()->end());
        }
        return &c->second;

    } else {
        assert(_flat_data_w != nullptr && row < _flat_data_ro->rows_size() && column < _flat_data_ro->column_count());
        return _flat_data_w->mutable_rows(row)->mutable_data(column);
    }
}

ValueWrapper::ValueWrapper(const ProtoShared::Value& v)
    : ValueWrapper(&v)
{
}

ValueWrapper::ValueWrapper(const ProtoShared::Value* v)
    : _data_ro(v)
{
    assert(v != nullptr);
}

ValueWrapper::ValueWrapper(ProtoShared::Value* v)
    : _data_w(v)
    , _data_ro(v)
{
    assert(v != nullptr);
}

ValueWrapper::ValueWrapper(ProtoShared::Value& v)
    : ValueWrapper(&v)
{
}

DataType ValueWrapper::type() const
{
    return type(_data_ro);
}

const std::string* ValueWrapper::toString() const
{
    assert(type() == DataType::String);
    return &_data_ro->v_string();
}

int64_t ValueWrapper::toInt() const
{
    assert(type() == DataType::Int);
    return _data_ro->v_int();
}

double ValueWrapper::toDouble() const
{
    assert(type() == DataType::Double);
    return _data_ro->v_double();
}

bool ValueWrapper::toBool() const
{
    assert(type() == DataType::Bool);
    return _data_ro->v_bool();
}

const std::string* ValueWrapper::toBytes() const
{
    assert(type() == DataType::Bytes);
    return &_data_ro->v_bytes();
}

uint64_t ValueWrapper::toDateTime() const
{
    assert(type() == DataType::DateTime);
    return _data_ro->v_date_time();
}

uint32_t ValueWrapper::toTime() const
{
    assert(type() == DataType::Time);
    return _data_ro->v_time();
}

bool ValueWrapper::isNull() const
{
    return type() == DataType::Null;
}

std::string ValueWrapper::convertToString() const
{
    return convertToString(_data_ro);
}

void ValueWrapper::setValue(int64_t v)
{
    setValue(_data_w, v);
}

void ValueWrapper::setValue(double v)
{
    setValue(_data_w, v);
}

void ValueWrapper::setValue(bool v)
{
    setValue(_data_w, v);
}

void ValueWrapper::setValue(const std::string& v)
{
    setValue(_data_w, v);
}

void ValueWrapper::setValue(const char* v)
{
    setValue(_data_w, std::string(v));
}

void ValueWrapper::setValue(const std::vector<char>& v)
{
    setValue(_data_w, v);
}

void ValueWrapper::setDateTime(uint64_t v)
{
    setDateTime(_data_w, v);
}

void ValueWrapper::setTime(uint32_t v)
{
    setTime(_data_w, v);
}

void ValueWrapper::setNull()
{
    setNull(_data_w);
}

void ValueWrapper::setValue(ProtoShared::Value* data, int64_t v)
{
    assert(data != nullptr);
    data->set_v_int(v);
}

void ValueWrapper::setValue(ProtoShared::Value* data, double v)
{
    assert(data != nullptr);
    data->set_v_double(v);
}

void ValueWrapper::setValue(ProtoShared::Value* data, bool v)
{
    assert(data != nullptr);
    data->set_v_bool(v);
}

void ValueWrapper::setValue(ProtoShared::Value* data, const std::string& v)
{
    assert(data != nullptr);
    data->set_v_string(v);
}

void ValueWrapper::setValue(ProtoShared::Value* data, const std::vector<char>& v)
{
    assert(data != nullptr);
    data->set_v_bytes(std::string(v.cbegin(), v.cend()));
}

void ValueWrapper::setDateTime(ProtoShared::Value* data, uint64_t v)
{
    assert(data != nullptr);
    data->set_v_date_time(v);
}

void ValueWrapper::setTime(ProtoShared::Value* data, uint32_t v)
{
    assert(data != nullptr);
    data->set_v_time(v);
}

void ValueWrapper::setNull(ProtoShared::Value* data)
{
    assert(data != nullptr);
    data->set_v_null(true);
}

ProtoShared::Value* ValueWrapper::data() const
{
    return _data_w;
}

const ProtoShared::Value* ValueWrapper::constData() const
{
    return _data_ro;
}

DataType ValueWrapper::type(const ProtoShared::Value* data)
{
    if (data == nullptr)
        return DataType::Empty;

    switch (data->data_case()) {
        case ProtoShared::Value::kVString:
            return DataType::String;

        case ProtoShared::Value::kVInt:
            return DataType::Int;

        case ProtoShared::Value::kVDouble:
            return DataType::Double;

        case ProtoShared::Value::kVBool:
            return DataType::Bool;

        case ProtoShared::Value::kVBytes:
            return DataType::Bytes;

        case ProtoShared::Value::kVNull:
            return DataType::Null;

        case ProtoShared::Value::kVDateTime:
            return DataType::DateTime;

        case ProtoShared::Value::kVTime:
            return DataType::Time;

        case ProtoShared::Value::DATA_NOT_SET:
            return DataType::Empty;
    }
    return DataType::Empty;
}

const std::string* ValueWrapper::toString(const ProtoShared::Value* data)
{
    assert(data != nullptr);
    return &data->v_string();
}

int64_t ValueWrapper::toInt(const ProtoShared::Value* data)
{
    assert(data != nullptr);
    return data->v_int();
}

double ValueWrapper::toDouble(const ProtoShared::Value* data)
{
    assert(data != nullptr);
    return data->v_double();
}

bool ValueWrapper::toBool(const ProtoShared::Value* data)
{
    assert(data != nullptr);
    return data->v_bool();
}

const std::string* ValueWrapper::toBytes(const ProtoShared::Value* data)
{
    assert(data != nullptr);
    return &data->v_bytes();
}

uint64_t ValueWrapper::toDateTime(const ProtoShared::Value* data)
{
    assert(data != nullptr);
    return data->v_date_time();
}

uint32_t ValueWrapper::toTime(const ProtoShared::Value* data)
{
    assert(data != nullptr);
    return data->v_time();
}

bool ValueWrapper::isNull(const ProtoShared::Value* data)
{
    assert(data != nullptr);
    return data->v_null();
}

std::string ValueWrapper::convertToString(const ProtoShared::Value* data)
{
    assert(data != nullptr);

    switch (data->data_case()) {
        case ProtoShared::Value::kVNull:
        case ProtoShared::Value::DATA_NOT_SET:
            return {};

        case ProtoShared::Value::kVString:
            return data->v_string();

        case ProtoShared::Value::kVInt:
            return std::to_string(data->v_int());

        case ProtoShared::Value::kVDouble:
            return std::to_string(data->v_double());

        case ProtoShared::Value::kVBool:
            return std::to_string(data->v_bool());

        case ProtoShared::Value::kVBytes:
            return data->v_bytes();

        case ProtoShared::Value::kVTime: {
            std::chrono::system_clock::time_point tp {std::chrono::seconds {data->v_time()}};
            return serializeTimePoint(tp, "%H:%M:%S");
        }

        case ProtoShared::Value::kVDateTime:
            std::chrono::system_clock::time_point tp {std::chrono::milliseconds {data->v_date_time()}};
            return serializeTimePoint(tp, "UTC: %Y-%m-%d %H:%M:%S");
    }

    return {};
}

DataContainerWrapper::DataContainerWrapper(const ProtoShared::DataContainer& container)
    : DataContainerWrapper(&container)
{
}

DataContainerWrapper::DataContainerWrapper(const ProtoShared::DataContainer* container)
    : _data_ro(container)
{
    assert(container != nullptr);
    init();
}

DataContainerWrapper::DataContainerWrapper(ProtoShared::DataContainer* container, bool packet_data)
    : _data_w(container)
    , _data_ro(container)
{
    assert(container != nullptr);
    _data_w->set_packet_data(packet_data);
    init();
}

DataContainerWrapper::DataContainerWrapper(ProtoShared::DataContainer& container, bool packet_data)
    : DataContainerWrapper(&container, packet_data)
{
}

void DataContainerWrapper::init()
{
    //    _arena = std::make_unique<google::protobuf::Arena>();
}

std::shared_ptr<DataContainerWrapper> DataContainerWrapper::createWriter(ProtoShared::DataContainer* container, bool packet_data)
{
    return std::shared_ptr<DataContainerWrapper>(new DataContainerWrapper(container, packet_data));
}

std::shared_ptr<DataContainerWrapper> DataContainerWrapper::createWriter(ProtoShared::DataContainer& container, bool packet_data)
{
    return std::shared_ptr<DataContainerWrapper>(new DataContainerWrapper(container, packet_data));
}

std::shared_ptr<DataContainerWrapper> DataContainerWrapper::createReader(const ProtoShared::DataContainer& container)
{
    return std::shared_ptr<DataContainerWrapper>(new DataContainerWrapper(container));
}

std::shared_ptr<DataContainerWrapper> DataContainerWrapper::createReader(const ProtoShared::DataContainer* container)
{
    return std::shared_ptr<DataContainerWrapper>(new DataContainerWrapper(container));
}

DataContainerWrapper::~DataContainerWrapper()
{
    for (auto i = _fields_w.cbegin(); i != _fields_w.cend(); ++i) {
        delete i->second;
    }
    for (auto i = _fields_ro.cbegin(); i != _fields_ro.cend(); ++i) {
        delete i->second;
    }
    for (auto i = _datasets_w.cbegin(); i != _datasets_w.cend(); ++i) {
        delete i->second;
    }
    for (auto i = _datasets_ro.cbegin(); i != _datasets_ro.cend(); ++i) {
        delete i->second;
    }
}

bool DataContainerWrapper::isPacketData() const
{
    return _data_ro->packet_data();
}

uint32_t DataContainerWrapper::version() const
{
    return _data_ro->structure_version();
}

void DataContainerWrapper::setVersion(uint32_t v)
{
    assert(_data_w != nullptr);
    _data_w->set_structure_version(v);
}

bool DataContainerWrapper::hasField(uint32_t id) const
{
    return _data_ro->fields().contains(id);
}

bool DataContainerWrapper::hasDataset(uint32_t id) const
{
    return _data_ro->datasets().contains(id);
}

ValueWrapper* DataContainerWrapper::fieldWriter(uint32_t id) const
{
    auto i = _fields_w.find(id);
    if (i != _fields_w.cend())
        return i->second;

    assert(_data_w != nullptr);
    auto f = _data_w->mutable_fields()->find(id);
    if (f == _data_w->mutable_fields()->end()) {
        (*_data_w->mutable_fields())[id].set_v_null(true);
        //        (*_data_w->mutable_fields())[id] = ProtoShared::Value();
        f = _data_w->mutable_fields()->find(id);
        assert(f != _data_w->mutable_fields()->end());
    }

    ValueWrapper* v = new ValueWrapper(f->second);
    _fields_w[id] = v;
    return v;
}

const ValueWrapper* DataContainerWrapper::fieldReader(uint32_t id) const
{
    auto i = _fields_ro.find(id);
    if (i != _fields_ro.cend())
        return i->second;

    auto f = _data_ro->fields().find(id);
    assert(f != _data_ro->fields().end());

    const ValueWrapper* v = new ValueWrapper(f->second);
    _fields_ro[id] = v;
    return v;
}

DatasetWrapper* DataContainerWrapper::initDataset(uint32_t id, size_t row_count, size_t column_count)
{
    DatasetWrapper* w;

    if (isPacketData()) {
        w = datasetWriter(id);
        w->grow(row_count, column_count);

    } else {
        w = datasetWriterHelper(id, true, row_count, column_count);
    }

    return w;
}

DatasetWrapper* DataContainerWrapper::datasetWriter(uint32_t id) const
{
    return datasetWriterHelper(id, false, 0, 0);
}

const DatasetWrapper* DataContainerWrapper::datasetReader(uint32_t id) const
{
    auto i = _datasets_ro.find(id);
    if (i != _datasets_ro.cend())
        return i->second;

    assert(_data_ro != nullptr);
    auto f = _data_ro->datasets().find(id);
    assert(f != _data_ro->datasets().end());

    const DatasetWrapper* v;
    if (isPacketData())
        v = new DatasetWrapper(f->second.packet());
    else
        v = new DatasetWrapper(f->second.flat());

    _datasets_ro[id] = v;
    return v;
}

ProtoShared::DataContainer* DataContainerWrapper::data() const
{
    return _data_w;
}

const ProtoShared::DataContainer* DataContainerWrapper::constData() const
{
    return _data_ro;
}

DatasetWrapper* DataContainerWrapper::datasetWriterHelper(uint32_t id, bool create, size_t row_count, size_t column_count) const
{
    auto i = _datasets_w.find(id);
    if (i != _datasets_w.cend())
        return i->second;

    assert(_data_w != nullptr);
    DatasetWrapper* v;

    if (isPacketData()) {
        auto f = _data_w->mutable_datasets()->find(id);
        if (f == _data_w->mutable_datasets()->end()) {
            if (_arena != nullptr)
                (*_data_w->mutable_datasets())[id].set_allocated_packet(
                    google::protobuf::Arena::CreateMessage<ProtoShared::PacketDataset>(_arena.get()));
            else
                (*_data_w->mutable_datasets())[id].set_allocated_packet(new ProtoShared::PacketDataset);

            f = _data_w->mutable_datasets()->find(id);
            assert(f != _data_w->mutable_datasets()->end());
        }

        v = new DatasetWrapper(f->second.mutable_packet());

    } else {
        auto f = _data_w->mutable_datasets()->find(id);
        if (f == _data_w->mutable_datasets()->end()) {
            if (create) {
                ProtoShared::FlatDataset* flat_ds;
                if (_arena != nullptr)
                    flat_ds = google::protobuf::Arena::CreateMessage<ProtoShared::FlatDataset>(_arena.get());
                else
                    flat_ds = new ProtoShared::FlatDataset;

                flat_ds->set_column_count(column_count);
                for (size_t row = 0; row < row_count; row++) {
                    auto fr = flat_ds->add_rows();
                    for (size_t col = 0; col < column_count; col++) {
                        fr->add_data();
                    }
                }

                (*_data_w->mutable_datasets())[id].set_allocated_flat(flat_ds);
                f = _data_w->mutable_datasets()->find(id);

            } else {
                assert(false); // необходимо сначала инициализировать через initDataset
            }
        }
        v = new DatasetWrapper(f->second.mutable_flat());
    }

    _datasets_w[id] = v;
    return v;
}

} // namespace api
