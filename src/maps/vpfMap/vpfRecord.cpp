#include "openeaagles/maps/vpfMap/VPFRecord.h"
#include "openeaagles/maps/vpfMap/VPFIndexTable.h"
#include "openeaagles/basic/String.h"

// disable all deprecation warnings for now, until we fix
// they are quite annoying to see over and over again...
#if(_MSC_VER>=1400)   // VC8+
# pragma warning(disable: 4996)
#endif

#include <fstream>

namespace Eaagles {
namespace Maps {
namespace Vpf {

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(VPFRecord,"VPFRecord")
EMPTY_SERIALIZER(VPFRecord)

VPFRecord::VPFRecord()
{
    STANDARD_CONSTRUCTOR()
    for (int i = 0; i < VPFTable::MAX_COLUMNS; i++) data[i] = 0;
    parent = 0;
    index = 0;
    eor = false;
    numCoords = 1;
    filename = 0;
}

void VPFRecord::copyData(const VPFRecord& org, const bool)
{
    BaseClass::copyData(org);
}

void VPFRecord::deleteData()
{
    for (int i = 0; i < VPFTable::MAX_COLUMNS; i++) {
        if (data[i] != 0) data[i]->unref();
        data[i] = 0;
    }
    if (parent != 0) {  
        parent->unref();
        parent = 0;
    }
}

void VPFRecord::createRecord(VPFTable* x, const char* file, const int idx) 
{
    if (parent != 0) parent->unref();
    parent = 0;
    if (x != 0) {
        parent = x;
        parent->ref();
    }
    if (filename == 0) filename = new Basic::String(file);
    else filename->setStr(file);
    index = idx;

    // ok, we have our table and our file, let's open it up, skip the header, 
    // grab the record, then load up our data, and return
    std::ifstream stream;
    stream.open(filename->getString());
    if (!stream.fail() && parent != 0) {
        // let's try to get the size of my file contents
        stream.seekg(0);
        int begin = stream.tellg();
        stream.seekg(0, std::ios::end);
        int end = stream.tellg();
        int mySize = end - begin;
        int size = parent->getRecordSize();
        if (size > 0) {
            int recordOffset = size * (idx - 1);
            // now offest our header, plus the index * size of one records
            stream.seekg(parent->getHeaderLength() + sizeof(int) + recordOffset, std::ios::beg);
            if (stream.tellg() >= mySize) {
                //std::cout << "TELL G = " << stream.tellg() << std::endl;
                //std::cout << "SIZE = " << mySize << std::endl;
                eor = true;
            }
            else {
                // now read our data
                bool rowNull = false;
                for (int i = 0; i < parent->getNumberOfColumns(); i++) {
                    parent->setCurrentColumn(i+1);
                    char dType = parent->getColumnDataType();
                    int num = atoi(parent->getColumnNumber());
                    // now compare the data types to figure our data
                    if (data[i] == 0) data[i] = new VPFDataType();
                    // long integer (4 bytes)
                    if (dType == 'I') {
                        // we read an integer
                        long int y = 0;
                        // 4 byte long integer
                        stream.read((char*)&y, num*sizeof(y));
                        // convert to string
                        char tempBuff[255];
                        sprintf(tempBuff, "%i", y);
                        //itoa(y, tempBuff, 10);
                        if (y == 0) rowNull = true;
                        // create a data type
                        if (rowNull) data[i]->setValue(NULL);
                        else data[i]->setValue(tempBuff);
                        data[i]->setLength(num*sizeof(y));
                        data[i]->setType(VPFDataType::LONG_INT);
                    }
                    // short integer (2 bytes)
                    else if (dType == 'S') {
                        // we read a short integer
                        short int y = 0;
                        // 4 byte long integer
                        stream.read((char*)&y, num*sizeof(y));
                        // convert to string
                        char tempBuff[255];
                        sprintf(tempBuff, "%i", y);
                        //itoa(y, tempBuff, 10);
                        if (rowNull) data[i]->setValue(NULL);
                        else data[i]->setValue(tempBuff);
                        data[i]->setLength(num*sizeof(y));
                        data[i]->setType(VPFDataType::SHORT_INT);
                    }
                    // text (fixed and variable length)
                    else if (dType == 'T') {
                        char buff[255];
                        stream.read(buff, num);
                        buff[num] = 0;
                        if (!rowNull) data[i]->setValue(buff);
                        else data[i]->setValue(NULL);
                        
                        data[i]->setLength(num);
                        data[i]->setType(VPFDataType::FIXED_TEXT);
                    }
                    else if (dType == 'D') {
                        // for every number of digits, we add 20 bits
                        // for the date
                        char buff[255];
                        stream.read(buff, num*20);
                        buff[num*20] = 0;
                        if (rowNull) data[i]->setValue(0);
                        else data[i]->setValue(buff);
                        data[i]->setLength(num*20);
                        data[i]->setType(VPFDataType::DATE);
                    }
                    // 4 byte floating point number
                    else if (dType == 'F') {
                        float tempFloat = 0;
                        char buff[255];
                        stream.read((char*)&tempFloat, num*sizeof(tempFloat));
                        //gcvt(tempFloat, 10, buff);  
                        sprintf(buff, "%f", tempFloat);
                        if (rowNull) data[i]->setValue(0);
                        else data[i]->setValue(buff);
                        data[i]->setLength(num*sizeof(tempFloat));
                        data[i]->setType(VPFDataType::FLOAT);
                    }
                    // 8 byte floating point (double)
                    else if (dType == 'R') {
                        double tempDouble = 0;
                        char buff[255];
                        stream.read((char*)&tempDouble, num*sizeof(tempDouble));
                        //gcvt(tempDouble, 10, buff);
                        sprintf(buff, "%f", tempDouble);
                        if (rowNull) data[i]->setValue(0);
                        else data[i]->setValue(buff);
                        data[i]->setLength(num*sizeof(tempDouble));
                        data[i]->setType(VPFDataType::DOUBLE);
                    }
                    // 3 coordinate array
                    else if (dType == 'Z') {
                        float tempFloat = 0;
                        Basic::String* string = new Basic::String();
                        char buff[255];
                        stream.read((char*)&tempFloat, sizeof(tempFloat));
                        //gcvt(tempFloat, 8, buff);
                        sprintf(buff, "%f", tempFloat);
                        string->catStr(buff);
                        string->catStr(", ");
                        stream.read((char*)&tempFloat, sizeof(tempFloat));
                        //gcvt(tempFloat, 8, buff);
                        sprintf(buff, "%f", tempFloat);
                        string->catStr(buff);
                        string->catStr(", ");
                        //stream.read((char*)&tempFloat, sizeof(tempFloat));
                        stream.read((char*)&tempFloat, sizeof(tempFloat));
                        // this is a quick and easy test for Quiet Not a Number, which we will just revert to 1
                        if (tempFloat != tempFloat) {
                            tempFloat = 1.0f;
                        }
                        //gcvt(tempFloat, 8, buff);
                        sprintf(buff, "%f", tempFloat);
                        string->catStr(buff);
                        string->catStr(" ");
                        //std::cout << "COORDINATE STRING = " << string->getString() << std::endl;

                        if (rowNull) data[i]->setValue(0);
                        else data[i]->setValue((char*)string->getString());
                        data[i]->setLength(sizeof(tempFloat)*3);
                        data[i]->setType(VPFDataType::THREE_D_COORD);
                        string->unref();
                    }
                    else {
                        std::cout << "UNKNOWN TYPE = " << dType << std::endl;
                        // clean up our data row, because we didn't use it
                        data[i]->unref();
                        data[i] = 0;
                    }
                }
            }
        }
        else {
            // must be an index file associated with it
            VPFIndexTable* t = parent->getIndexTable();
            if (t != 0) {
                int offset = 0, length = 0;
                t->getRecordPosition(idx, offset, length);
                // now we know the record position, we can open our file and parse it appropriately
                stream.seekg(offset, std::ios::beg);
                //std::cout << "OFFSET / LENGTH = " << offset << ", " << length << std::endl;
                // now read our data
                bool rowNull = false;
                bool vLength = false;
                for (int i = 0; i < parent->getNumberOfColumns(); i++) {
                    vLength = false;
                    parent->setCurrentColumn(i+1);
                    char dType = parent->getColumnDataType();
                    int num = atoi(parent->getColumnNumber());
                    if (num == 0) {
                        // this column is variable length!
                        vLength = true;
                    }
                    // now compare the data types to figure our data
                    if (data[i] == 0) data[i] = new VPFDataType();
                    // long integer (4 bytes)
                    if (dType == 'I') {
                        // we read an integer
                        long int y = 0;
                        // 4 byte long integer
                        stream.read((char*)&y, num*sizeof(y));
                        // convert to string
                        char tempBuff[255];
                        sprintf(tempBuff, "%i", y);
                        //itoa(y, tempBuff, 10);
                        if (y == 0) rowNull = true;
                        // create a data type
                        if (rowNull) data[i]->setValue(0);
                        else data[i]->setValue(tempBuff);
                        data[i]->setLength(num*sizeof(y));
                        data[i]->setType(VPFDataType::LONG_INT);
                    }
                    // short integer (2 bytes)
                    else if (dType == 'S') {
                        // we read a short integer
                        short int y = 0;
                        // 4 byte long integer
                        stream.read((char*)&y, num*sizeof(y));
                        // convert to string
                        char tempBuff[255];
                        sprintf(tempBuff, "%i", y);                        
                        //itoa(y, tempBuff, 10);
                        if (rowNull) data[i]->setValue(0);
                        else data[i]->setValue(tempBuff);
                        data[i]->setLength(num*sizeof(y));
                        data[i]->setType(VPFDataType::SHORT_INT);
                    }
                    // text (fixed and variable length)
                    else if (dType == 'T') {
                        char buff[255];
                        if (vLength) {
                            stream.read(buff, num);
                            buff[num] = 0;
                            data[i]->setLength(1);
                            data[i]->setType(VPFDataType::VARIABLE_TEXT);
                        }
                        else {
                            stream.read(buff, num);
                            buff[num] = 0;
                            data[i]->setLength(num);
                            data[i]->setType(VPFDataType::FIXED_TEXT);
                        }
                        if (!rowNull) data[i]->setValue(buff);
                        else data[i]->setValue(0);
                        
                    }
                    else if (dType == 'D') {
                        // for every number of digits, we add 20 bits
                        // for the date
                        char buff[255];
                        stream.read(buff, num*20);
                        buff[num*20] = 0;
                        if (rowNull) data[i]->setValue(0);
                        else data[i]->setValue(buff);
                        data[i]->setLength(num*20);
                        data[i]->setType(VPFDataType::DATE);
                    }
                    // 4 byte floating point number
                    else if (dType == 'F') {
                        float tempFloat = 0;
                        char buff[255];
                        stream.read((char*)&tempFloat, num*sizeof(tempFloat));
                        //gcvt(tempFloat, 10, buff);
                        sprintf(buff, "%f", tempFloat);
                        if (rowNull) data[i]->setValue(0);
                        else data[i]->setValue(buff);
                        data[i]->setLength(num*sizeof(tempFloat));
                        data[i]->setType(VPFDataType::FLOAT);
                    }
                    // 8 byte floating point (double)
                    else if (dType == 'R') {
                        double tempDouble = 0;
                        char buff[255];
                        stream.read((char*)&tempDouble, num*sizeof(tempDouble));
                        //gcvt(tempDouble, 10, buff);
                        sprintf(buff, "%f", tempDouble);
                        if (rowNull) data[i]->setValue(0);
                        else data[i]->setValue(buff);
                        data[i]->setLength(num*sizeof(tempDouble));
                        data[i]->setType(VPFDataType::DOUBLE);
                    }
                    // 3 coordinate array
                    else if (dType == 'Z') {
                        if (vLength) {
                            // the first length of the first part of our vLength field is the number of coordinate pairs we have
                            stream.read((char*)&numCoords, sizeof(numCoords));
                            //std::cout << "NUM COORDS = " << numCoords << std::endl;
                            char buff[255];
                            float tempFloat = 0;
                            Basic::String* string = new Basic::String();
                            for (int j = 0; j < numCoords; j++) {
                                stream.read((char*)&tempFloat, sizeof(tempFloat));
                                //gcvt(tempFloat, 8, buff);
                                sprintf(buff, "%f", tempFloat);
                                string->catStr(buff);
                                string->catStr(" ");
                                stream.read((char*)&tempFloat, sizeof(tempFloat));
                                //gcvt(tempFloat, 8, buff);
                                sprintf(buff, "%f", tempFloat);
                                string->catStr(buff);
                                string->catStr(" ");
                                stream.read((char*)&tempFloat, sizeof(tempFloat));
                                //gcvt(tempFloat, 8, buff);
                                sprintf(buff, "%f", tempFloat);
                                string->catStr(buff);
                                string->catStr(" ");
                            }
                            if (rowNull) data[i]->setValue(0);
                            else data[i]->setValue((char*)string->getString());
                            data[i]->setLength(numCoords * 3);
                            data[i]->setType(VPFDataType::THREE_D_COORD);
                            //std::cout << "COORDINATES = " << string->getString() << std::endl;
                            string->unref();
                        }
                        else {
                            numCoords = 1;
                            float tempFloat = 0;
                            Basic::String* string = new Basic::String();
                            char buff[255];
                            stream.read((char*)&tempFloat, sizeof(tempFloat));
                            //gcvt(tempFloat, 8, buff);
                            sprintf(buff, "%f", tempFloat);
                            string->catStr(buff);
                            string->catStr(" ");
                            stream.read((char*)&tempFloat, sizeof(tempFloat));
                            //gcvt(tempFloat, 8, buff);
                            sprintf(buff, "%f", tempFloat);
                            string->catStr(buff);
                            string->catStr(" ");
                            stream.read((char*)&tempFloat, sizeof(tempFloat));
                            //gcvt(tempFloat, 8, buff);
                            sprintf(buff, "%f", tempFloat);
                            string->catStr(buff);
                            string->catStr(" ");
                            if (rowNull) data[i]->setValue(0);
                            else data[i]->setValue((char*)string->getString());
                            data[i]->setLength(sizeof(tempFloat)*3);
                            data[i]->setType(VPFDataType::THREE_D_COORD);
                            std::cout << "COORDINATE = " << string->getString() << std::endl;
                            string->unref();
                        }
                    }
                    else {
                        std::cout << "UNKNOWN TYPE = " << dType << std::endl;
                        // clean up our data row, because we didn't use it
                        data[i]->unref();
                        data[i] = 0;
                    }
                }
            }
            else {
                std::cout << "ERROR: VPFTable has no index table and no associated record!" << std::endl;
            }
        }
        stream.close();
    }
    else {
        std::cout << "FAILURE TO OPEN FILE = " << filename->getString() << std::endl;
    }
}


// This function is for retrieving variable length coordinate strings, and
// will return the number of coordinates as well as an osg::Vec3 array filled
// with those coordinates
int VPFRecord::getCoordinate(const int column, osg::Vec3 vec[], const int idx, const int max)
{
    // first, get the data from the column
    if (data[column-1] != 0) {
        if (data[column-1]->getType() == VPFDataType::THREE_D_COORD) {
            // ok, they have a valid column, now lets convert our string into 3 coordinates (x,y,z from center)
            float first = 0, second = 0, third = 0;
            // grab our first coordinate
            char temp[255];
            char* x = (char*)data[column-1]->getValue();
            int j = idx;
            int count = 0;
            int tCount = 0;
            for (int i = 0; i < numCoords; i++) {
                tCount = 0;
                while (x[count] != ' ') {
                    temp[tCount++] = x[count++];
                }
                temp[tCount] = 0;

                first = (float)atof(temp);
                // our first value is our LONGITUDE!
                // now, moving on from that first coordinate, we go to our second one
                temp[0] = 0;
                tCount = 0;
                count++;
                while (x[count] != ' ') temp[tCount++] = x[count++];
                temp[tCount] = 0;
                second = (float)atof(temp);
                //if ((first <= 45 && first >= 44) && (second >= 33 && second <= 34)) {
                    //std::cout << "IDX = " << idx+1 << std::endl;
                    //std::cout << "NUM COORDINATES = " << numCoords << std::endl;
                    //std::cout << "PRIMITIVE ID = " << data[1]->getValue() << std::endl;
                    //std::cout << "LON = " << first << std::endl;
                    //std::cout << "LAT = " << second << std::endl;
                //}
                // now the third coordinate, which right now is just 1
                tCount = 0;
                temp[0] = 0;
                count++;
                while(x[count] != ' ') temp[tCount++] = x[count++];
                temp[tCount] = 0;
                //std::cout << "THIRD = " << temp << std::endl;
                third = (float)atof(temp);
                count++;
                if (j < max) vec[j++].set(second,first,third);
                j++;
            }
        }
    }
    return numCoords;
}


// this just returns the character string that was parsed from the file
const char* VPFRecord::getData(const int column)
{
    if (column < VPFTable::MAX_COLUMNS && data[column-1] != 0) {
        //std::cout << "RECORD # & DATA = " << index << ", " << data[column-1]->getValue() << std::endl;
        return data[column-1]->getValue();
    }
    else return 0;
}

}; // end Vpf namespace
}; // end Maps namespace 
}; // end Eaagles namespace
