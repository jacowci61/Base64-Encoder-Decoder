#include <iostream>
#include <sstream>
#include <bitset>
#include <string>
#include <vector>
#include <regex>
#include <map>
using namespace std;

int main()
{
    map<int, char> B64CharTable =
    {
        {0,'A'},{1,'B'},{2,'C'},{3,'D'},{4,'E'},{5,'F'},{6,'G'},{7,'H'},{8,'I'},{9,'J'},{10,'K'},{11,'L'},{12,'M'},{13,'N'},
        {14,'O'},{15,'P'},{16,'Q'},{17,'R'},{18,'S'},{19,'T'},{20,'U'},{21,'V'},{22,'W'},{23,'X'},{24,'Y'},{25,'Z'},
        {26,'a'},{27,'b'},{28,'c'},{29,'d'},{30,'e'},{31,'f'},{32,'g'},{33,'h'},{34,'i'},{35,'j'},{36,'k'},{37,'l'},{38,'m'},
        {39,'n'},{40,'o'},{41,'p'},{42,'q'},{43,'r'},{44,'s'},{45,'t'},{46,'u'},{47,'v'},{48,'w'},{49,'x'},{50,'y'},{51,'z'},
        {52,'0'},{53,'1'},{54,'2'},{55,'3'},{56,'4'},{57,'5'},{58,'6'},{59,'7'},{60,'8'},{61,'9'},
        {62,'+'},{63,'/'}
    };

    ostringstream OutputStream;

    string EightByte_BinaryBuffer;
    string SixByte_BinaryBuffer;
    string EncodeOrDecode;
    string DecimalOutput;
    string InputBuffer;
    string SmallBuffer;
    string FinalResult;
    string Temp;

    int CharCounter = 0;


    cout << "To encode text enter 0, to decode text enter 1: "; // encode - 0; decode - 1
    getline(cin, EncodeOrDecode);

    while ((EncodeOrDecode == "0" || EncodeOrDecode == "1") == false)
    {
        EncodeOrDecode.clear();
        cout << "Incorrect input data,try again." << endl;
        cout << "To encode text enter 0, to decode text enter 1: "; //encode - 0; decode - 1
        getline(cin, EncodeOrDecode); //convert string to char array

        if (EncodeOrDecode == "0" || EncodeOrDecode == "1")
        {
            break;
        }
    }



    if (EncodeOrDecode == "0")
    {
        cout << "Enter string to encode: ";
        getline(cin, InputBuffer);

        for (char& InputChar : InputBuffer)
        {
            SixByte_BinaryBuffer += bitset<8>(InputChar).to_string();
        }
        InputBuffer.clear();


        for (int i = 0; i < SixByte_BinaryBuffer.length(); i++)
        {
            if (CharCounter == 5 && i != SixByte_BinaryBuffer.length() - 1)
            {
                CharCounter = 0;
                EightByte_BinaryBuffer += SixByte_BinaryBuffer[i];
                EightByte_BinaryBuffer += " ";
            }
            else if (i + 1 == SixByte_BinaryBuffer.length() && CharCounter != 5)
            {
                for (int i = 1; i < (5 - CharCounter); i++)
                {
                    EightByte_BinaryBuffer += "0";
                }
            }
            else
            {
                EightByte_BinaryBuffer += SixByte_BinaryBuffer[i];
                CharCounter++;
            }
        }

        for (int i = 0; i < EightByte_BinaryBuffer.length(); i++)
        {
            if (i == 0)
            {
                EightByte_BinaryBuffer.insert(i, "00");
                i += 2;
            }
            else if (EightByte_BinaryBuffer[i + 1] == ' ')
            {
                EightByte_BinaryBuffer.insert(i + 2, "00");
                i += 2;
            }
        }
        SixByte_BinaryBuffer.clear();

        for (int i = 0; i < EightByte_BinaryBuffer.length(); i++)
        {
            if (EightByte_BinaryBuffer[i] != ' ')
            {
                Temp += EightByte_BinaryBuffer[i];
            }
            else if (EightByte_BinaryBuffer[i] == ' ')
            {
                bitset<8> SmallBuffer(Temp);
                OutputStream << SmallBuffer.to_ulong() << endl;
                Temp.clear();
                SmallBuffer.reset();
            }
            if (i == EightByte_BinaryBuffer.length() - 1)
            {
                bitset<8> SmallBuffer(Temp);
                OutputStream << SmallBuffer.to_ulong() << endl;
                Temp.clear();
                SmallBuffer.reset();;
            }
        }
        DecimalOutput = OutputStream.str();
        DecimalOutput = regex_replace(DecimalOutput, regex("\n"), " ");

        for (int i = 0; i < DecimalOutput.length(); i++)
        {
            if (DecimalOutput[i] != ' ')
            {
                Temp += DecimalOutput[i];
            }
            else if (DecimalOutput[i] == ' ')
            {
                FinalResult += B64CharTable[stoi(Temp)];
                Temp.clear();
            }
        }
        cout << FinalResult << endl;
        FinalResult.clear();
        Temp.clear();
        SmallBuffer.clear();
    }
    else if (EncodeOrDecode == "1")
    {
        cout << "Enter string to decode: ";
        getline(cin, InputBuffer);
        vector<char> InputInChars(InputBuffer.begin(), InputBuffer.end());
        for (const char& i : InputInChars)
        {
            auto it = find_if
            (
                begin(B64CharTable), end(B64CharTable),
                [&i](const auto& p)
                {
                    return p.second == i;
                }
            );

            if (it != end(B64CharTable))
            {
                SmallBuffer += bitset<8>(it->first).to_string().erase(0, 2);
            }
        }

        for (int i = 0; i < SmallBuffer.length(); i++)
        {
            if (CharCounter == 7)
            {
                Temp += SmallBuffer[i];
                bitset <8> bits(Temp);
                FinalResult += bits.to_ulong();
                Temp.clear();
                CharCounter = 0;
            }
            else
            {
                Temp += SmallBuffer[i];
                CharCounter++;
            }
        }
        cout << FinalResult << endl;
        Temp.clear();
        SmallBuffer.clear();
        FinalResult.clear();
    }
}