#pragma once

#include "../generated/shared/shared.pb.h"
#include <google/protobuf/arena.h>

namespace api
{
enum class DataType
{
    Empty,
    Null,
    String,
    Int,
    Double,
    Bool,
    Bytes,
    DateTime,
    Time,
};

class ValueWrapper
{
public:
    DataType type() const;
    const std::string* toString() const;
    int64_t toInt() const;
    double toDouble() const;
    bool toBool() const;
    const std::string* toBytes() const;
    //! Время в миллисекундах с начала "эпохи"
    uint64_t toDateTime() const;
    //! Количество секунд
    uint32_t toTime() const;
    bool isNull() const;
    //! Преобразовать в строку, вне зависимости от содержимого
    std::string convertToString() const;

    void setValue(int64_t v);
    void setValue(double v);
    void setValue(bool v);
    void setValue(const std::string& v);
    void setValue(const char* v);
    void setValue(const std::vector<char>& v);
    void setDateTime(uint64_t v);
    void setTime(uint32_t v);
    void setNull();

    static DataType type(const ProtoShared::Value* data);
    static const std::string* toString(const ProtoShared::Value* data);
    static int64_t toInt(const ProtoShared::Value* data);
    static double toDouble(const ProtoShared::Value* data);
    static bool toBool(const ProtoShared::Value* data);
    static const std::string* toBytes(const ProtoShared::Value* data);
    static uint64_t toDateTime(const ProtoShared::Value* data);
    static uint32_t toTime(const ProtoShared::Value* data);
    static bool isNull(const ProtoShared::Value* data);
    static std::string convertToString(const ProtoShared::Value* data);

    static void setValue(ProtoShared::Value* data, int64_t v);
    static void setValue(ProtoShared::Value* data, double v);
    static void setValue(ProtoShared::Value* data, bool v);
    static void setValue(ProtoShared::Value* data, const std::string& v);
    static void setValue(ProtoShared::Value* data, const std::vector<char>& v);
    static void setDateTime(ProtoShared::Value* data, uint64_t v);
    static void setTime(ProtoShared::Value* data, uint32_t v);
    static void setNull(ProtoShared::Value* data);

    //! "сырые" данные (может быть nullptr)
    ProtoShared::Value* data() const;
    //! "сырые" данные
    const ProtoShared::Value* constData() const;

private:
    ValueWrapper(const ProtoShared::Value& v);
    ValueWrapper(const ProtoShared::Value* v);
    ValueWrapper(ProtoShared::Value* v);
    ValueWrapper(ProtoShared::Value& v);

    ProtoShared::Value* _data_w = nullptr;
    const ProtoShared::Value* _data_ro = nullptr;

    friend class DatasetWrapper;
    friend class DataContainerWrapper;
};

//! Обертка вокруг набора данных protobuffers
//! Не владеет содержимым (ничего не удаляет в деструкторе)
class DatasetWrapper
{
public:
    //! Количество строк
    size_t rowCount() const;
    //! Количество колонок
    size_t columnCount() const;

    //! Есть ли такая строка в памяти
    bool hasRow(size_t row) const;

    //! Увеличивает (при необходимости) общее количество строк и колонок
    //! Метод не обязателен, т.к. setValue автоматически выставляет нужное количество
    void grow(size_t row_count, size_t column_count);

    //! Лежит ли значение в ячейке
    bool hasValue(size_t row, size_t column) const;
    //! Тип данных в ячейке
    DataType type(size_t row, size_t column) const;
    ;
    const std::string* toString(size_t row, size_t column) const;
    int64_t toInt(size_t row, size_t column) const;
    double toDouble(size_t row, size_t column) const;
    bool toBool(size_t row, size_t column) const;
    const std::string* toBytes(size_t row, size_t column) const;
    //! Время в миллисекундах с начала "эпохи"
    uint64_t toDateTime(size_t row, size_t column) const;
    //! Количество секунд
    uint32_t toTime(size_t row, size_t column) const;
    bool isNull(size_t row, size_t column) const;
    //! Преобразовать в строку, вне зависимости от содержимого
    std::string convertToString(size_t row, size_t column) const;

    void setValue(size_t row, size_t column, int64_t v);
    void setValue(size_t row, size_t column, double v);
    void setValue(size_t row, size_t column, bool v);
    void setValue(size_t row, size_t column, const std::string& v);
    void setValue(size_t row, size_t column, const char* v);
    void setValue(size_t row, size_t column, const std::vector<char>& v);
    void setDateTime(size_t row, size_t column, uint64_t v);
    void setTime(size_t row, size_t column, uint32_t v);
    void setNull(size_t row, size_t column);

    //! Очистить данные в ячейке
    void clearValue(size_t row, size_t column);

    //! "сырые" данные (может быть nullptr)
    ProtoShared::PacketDataset* data() const;
    //! "сырые" данные
    const ProtoShared::PacketDataset* constData() const;

private:
    //! Только для чтения. Упакованный набор данных. Экономит память, но тормозит
    DatasetWrapper(const ProtoShared::PacketDataset& dataset);
    //! Только для чтения. Упакованный набор данных. Экономит память, но тормозит
    DatasetWrapper(const ProtoShared::PacketDataset* dataset);
    //! Чтение и запись. Упакованный набор данных. Экономит память, но тормозит
    DatasetWrapper(ProtoShared::PacketDataset* dataset);
    //! Чтение и запись. Упакованный набор данных. Экономит память, но тормозит
    DatasetWrapper(ProtoShared::PacketDataset& dataset);

    //! Только для чтения. Непрерывная область памяти. Быстро, но под свободные ячейки выделяется память
    DatasetWrapper(const ProtoShared::FlatDataset& dataset);
    //! Только для чтения. Непрерывная область памяти. Быстро, но под свободные ячейки выделяется память
    DatasetWrapper(const ProtoShared::FlatDataset* dataset);
    //! Чтение и запись. Непрерывная область памяти. Быстро, но под свободные ячейки выделяется память
    DatasetWrapper(ProtoShared::FlatDataset* dataset);
    //! Чтение и запись. Непрерывная область памяти. Быстро, но под свободные ячейки выделяется память
    DatasetWrapper(ProtoShared::FlatDataset& dataset);

    const ProtoShared::Value* rawValue(size_t row, size_t column, bool check) const;
    ProtoShared::Value* rawValue(size_t row, size_t column, bool create);

    ProtoShared::PacketDataset* _packet_data_w = nullptr;
    const ProtoShared::PacketDataset* _packet_data_ro = nullptr;

    ProtoShared::FlatDataset* _flat_data_w = nullptr;
    const ProtoShared::FlatDataset* _flat_data_ro = nullptr;

    friend class DataContainerWrapper;
};

class DataContainerWrapper;
using DataContainerWrapperPtr = std::shared_ptr<DataContainerWrapper>;

//! Контейнер с данными
class DataContainerWrapper
{
public:
    static DataContainerWrapperPtr createWriter(ProtoShared::DataContainer* container, bool packet_data);
    static DataContainerWrapperPtr createWriter(ProtoShared::DataContainer& container, bool packet_data);
    static DataContainerWrapperPtr createReader(const ProtoShared::DataContainer& container);
    static DataContainerWrapperPtr createReader(const ProtoShared::DataContainer* container);

    ~DataContainerWrapper();

    bool isPacketData() const;

    uint32_t version() const;
    void setVersion(uint32_t v);

    bool hasField(uint32_t id) const;
    bool hasDataset(uint32_t id) const;

    ValueWrapper* fieldWriter(uint32_t id) const;
    const ValueWrapper* fieldReader(uint32_t id) const;

    //! Инициализация набора данных. Имеет смысл только для не упакованных даннеы
    DatasetWrapper* initDataset(uint32_t id, size_t row_count, size_t column_count);
    DatasetWrapper* datasetWriter(uint32_t id) const;

    const DatasetWrapper* datasetReader(uint32_t id) const;

    //! "сырые" данные (может быть nullptr)
    ProtoShared::DataContainer* data() const;
    //! "сырые" данные
    const ProtoShared::DataContainer* constData() const;

private:
    DataContainerWrapper(const ProtoShared::DataContainer& container);
    DataContainerWrapper(const ProtoShared::DataContainer* container);
    DataContainerWrapper(ProtoShared::DataContainer* container, bool packet_data);
    DataContainerWrapper(ProtoShared::DataContainer& container, bool packet_data);

    void init();

    DatasetWrapper* datasetWriterHelper(uint32_t id, bool create, size_t row_count, size_t column_count) const;

    ProtoShared::DataContainer* _data_w = nullptr;
    const ProtoShared::DataContainer* _data_ro = nullptr;

    mutable std::map<uint32_t, ValueWrapper*> _fields_w;
    mutable std::map<uint32_t, const ValueWrapper*> _fields_ro;

    mutable std::map<uint32_t, DatasetWrapper*> _datasets_w;
    mutable std::map<uint32_t, const DatasetWrapper*> _datasets_ro;

    mutable std::unique_ptr<google::protobuf::Arena> _arena;
};

} // namespace api
