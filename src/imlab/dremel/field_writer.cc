// ---------------------------------------------------------------------------
// IMLAB
// ---------------------------------------------------------------------------
#include "imlab/dremel/field_writer.h"
//---------------------------------------------------------------------------
namespace imlab {
namespace dremel {
//---------------------------------------------------------------------------

void WriteField(FieldWriter* writer, const ProtoFieldValue& value, unsigned repetition_level) {
    if (!value.HasValue()) {
        writer->write(repetition_level);
        return;
    }

    switch (value.GetField()->cpp_type()) {
        case FieldDescriptor::CPPTYPE_INT32:
            dynamic_cast<AtomicFieldWriter<Integer>*>(writer)->write_value(
                Integer(value.GetFieldValue<int32_t>()), repetition_level);
            break;
        case FieldDescriptor::CPPTYPE_INT64:
            dynamic_cast<AtomicFieldWriter<Integer>*>(writer)->write_value(
                Integer(value.GetFieldValue<int64_t>()), repetition_level);
            break;
        case FieldDescriptor::CPPTYPE_UINT32:
            dynamic_cast<AtomicFieldWriter<Integer>*>(writer)->write_value(
                Integer(value.GetFieldValue<uint32_t>()), repetition_level);
            break;
        case FieldDescriptor::CPPTYPE_UINT64:
            dynamic_cast<AtomicFieldWriter<Integer>*>(writer)->write_value(
                Integer(value.GetFieldValue<uint64_t>()), repetition_level);
            break;
        case FieldDescriptor::CPPTYPE_STRING:
            dynamic_cast<AtomicFieldWriter<Varchar<30>>*>(writer)->write_value(
                Varchar<30>::build(value.GetFieldValue<std::string>().c_str()), repetition_level);
            break;
        case FieldDescriptor::CPPTYPE_DOUBLE:  // UNSUPPRTED
        case FieldDescriptor::CPPTYPE_FLOAT:  // UNSUPPRTED
        case FieldDescriptor::CPPTYPE_BOOL:  // UNSUPPRTED
        case FieldDescriptor::CPPTYPE_ENUM:  // UNSUPPRTED
        case FieldDescriptor::CPPTYPE_MESSAGE:  // INVALID
            break;
    }
}

//---------------------------------------------------------------------------
}  // namespace dremel
}  // namespace imlab
//---------------------------------------------------------------------------
