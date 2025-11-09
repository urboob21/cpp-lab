#include <iostream>
#include <string>

using namespace std;
// A type defined in terms of other types

// *0. Define a struct:
// structure alignment, padding, decreasing order of size
// public access
// In C, structs can only hold data members, not functions.
struct StructDataType
{
    /* data */
    double voltage{0.0}; // Default initialization
    int id{0};
    char status{'N'};
    string label{"Unknown"};

    /* function */
    void print() const
    {
        cout << "Size of struct = " << sizeof(StructDataType) << " bytes" << endl;
        cout << "Sensor " << id
             << " [" << label << "] "
             << "Voltage: " << voltage
             << " Status: " << status << endl;
    }
};

void updateVoltageRef(StructDataType &data, double newV)
{
    data.voltage = newV;
}

void updateVoltagePtr(StructDataType *data, double newV)
{
    // (*data).voltage = newV;
    if (data)
        data->voltage = newV;
}

StructDataType makeSensor(int id, double v, const string &label)
{
    return {v, id, 'N', label};
}

void structs()
{
    cout << "\n--- Struct Type Examples ---\n";
    // *1. Using struct
    [[maybe_unused]] StructDataType data0; // default init
    StructDataType data1{3.3, 1, 'N', "Temp1"};
    StructDataType data2 = {6.3, 2, 'N', "Temp2"};

    // *2. Access members
    data1.print();
    const StructDataType *ptr_data1 = &data1;
    ptr_data1->print();

    // *3. Passing by reference
    updateVoltageRef(data2, 100);
    data2.print();
    // *4. Passing by pointer
    updateVoltagePtr(&data2, 200);
    data2.print();

    // *5. Return a temporary struct
    StructDataType s2 = makeSensor(2, 5.0, "Pressure");
    s2.print();
}

// A struct that runs code when its object is created
struct CStruct
{
    CStruct()
    {
        cout << "\n"
             << "\n"
             << "Compound type: Struct\n";

        structs();
    }
};

// All global and static objects are constructed before main() begins.
static CStruct autoRunInstance;
