#include <iostream>
#include <Windows.h> 

// Prototypes
void QuestionSolution2();
void QuestionSolution3();
void QuestionSolution4();
bool ReadInputButton();
void ProcessArray(int* ArrayInput, int nSizeArrayInput,int* ArrayOutput, int& nSizeArrayOutput,int& MinimumNumber, int& MaximumNumber,float& Average);
void ISR_PacketReceived(char* Data, int DataSize);
void ProcessIncommingCommand(char Command, char* Data, int DataSize);

// Main program and question selector
int main()
{
    std::cout << "Enter the question number (2 to 4):";
    std::string question;
    std::cin >> question;

    if (question == "2")
    {
        QuestionSolution2();
    }
    else if (question == "3")
    {
        QuestionSolution3();
    }
    else if (question == "4")
    {
        QuestionSolution4();
    }
    else
    {
        std::cout << "Invalid question number." << "\n\n\n";
    }
}

// Question 2 section code - Begin
void QuestionSolution2()
{
    enum StateMachine
    {
        State_OFF,
        State_ON,
        State_PROTECTED
    };
    
    // Defines the initial state
    StateMachine StateMachine1 = State_OFF;
    
    std::cout << "State_OFF" << std::endl;

    // State machine processing (main loop)
    while (true)
    {
        switch (StateMachine1)
        {
            case State_OFF:
            {
                // Checks if button is pressed
                if (ReadInputButton() == true)
                {
                    std::cout << "State_ON" << std::endl;

                    StateMachine1 = State_ON;
                }

                break;
            }

            case State_ON:
            {
                // Checks if button is released
                if (ReadInputButton() == false)
                {
                    std::cout << "State_PROTECTED" << std::endl;

                    StateMachine1 = State_PROTECTED;
                }

                break;
            }

            case State_PROTECTED:
            {
                // Wait for 10 seconds
                Sleep(10000);

                std::cout << "State_OFF" << std::endl;

                // Return to initial state
                StateMachine1 = State_OFF;

                break;
            }
        }
    }
}

// Read input port simulation
bool ReadInputButton()
{
    std::cout << "Enter the inputt button value (0 or 1):";
    std::string input;
    std::cin >> input;

    if (input == "1")
    {
        return true;
    }

    return false;
}
// Question 2 section code - End



// Question 3 section code - Begin
void QuestionSolution3()
{
    // Input array to be processed 
    int ArrayInput[] = {-200,205,400,0,127,346};

    // Number of elements of input array
    int nSizeArrayInput = sizeof(ArrayInput) / sizeof(int);

    // Output array for even numbers (not initiated)
    int ArrayOutput[sizeof(ArrayInput) / sizeof(int)];

    // Number of elements of output array, starting with zero
    int nSizeArrayOutput = 0;

    // Maximum and minimum values of array
    int MaximumNumber, MinimumNumber;

    // Average of numbers in the array
    float Average;

    // Starts processing the input array
    ProcessArray(ArrayInput, nSizeArrayInput, ArrayOutput, nSizeArrayOutput, MinimumNumber, MaximumNumber, Average);

    // Print the results
    std::cout <<  std::endl << "ArrayInput:";
    for (int i = 0; i < nSizeArrayInput; i++) { std::cout << ArrayInput[i] << ","; }
    std::cout << std::endl << std::endl;
    std::cout << "Results:" << std::endl;
    std::cout << "MaximumNumber: " << MaximumNumber << std::endl;
    std::cout << "MinimumNumber: " << MinimumNumber << std::endl;
    std::cout << "Average: " << Average << std::endl;
    std::cout << "nSizeArrayOutput: " << nSizeArrayOutput << std::endl;
    std::cout << "ArrayOutput: " ;
    for (int i = 0; i < nSizeArrayOutput; i++) {std::cout << ArrayOutput[i] << ",";}
    std::cout << std::endl;
}

void ProcessArray(int* ArrayInput, int nSizeArrayInput,
    int* ArrayOutput, int& nSizeArrayOutput,
    int& MinimumNumber, int& MaximumNumber,
    float& Average)
{
    nSizeArrayOutput = 0;
    MinimumNumber = INT_MAX;
    MaximumNumber = INT_MIN;
    Average = 0;

    for(int i = 0; i < nSizeArrayInput; i++)
    {
        // Look for even numbers
        if (*ArrayInput % 2 == 0)
        {
            *ArrayOutput = *ArrayInput;

            ArrayOutput++;

            nSizeArrayOutput++;
        }

        // Look for the maximum number
        if (*ArrayInput > MaximumNumber)
        {
            MaximumNumber = *ArrayInput;
        }

        // Look for the minimum number
        if (*ArrayInput < MinimumNumber)
        {
            MinimumNumber = *ArrayInput;
        }

        Average += *ArrayInput;

        ArrayInput++;
    }

    Average = Average / nSizeArrayInput;

}
// Question 3 section code - End


// Question 4 section code - Begin
void QuestionSolution4()
{    
    // Emulates a Master device sending a packet to ask some data to a Slave device 
    // The following code just an example of packet (Addressed to slave device_ID=1, command_ID=5, and no data inside):
    //                     STX,ADDR,CMD,SIZE_DATA,CHECKSUM        
    char DataPacket[] = { 0x02,0x01,0x05,0x00,0x08 }; 
    int SizeDataPacket = sizeof(DataPacket);

    // Emulates a Slave device receiving a packet and calling a interruption routine to process the incoming data
    ISR_PacketReceived(DataPacket, SizeDataPacket);

}

void ISR_PacketReceived(char* Data,int DataSize)
{
    // Defines the slave device address
    #define MY_ADRESS 0x01 
    
    // Defines the header of packet
    typedef struct
    {
        char Stx;
        char AddressID;
        char CommandID;
        char SizeData;
        char Checksum;
    }PacketHeader;

    // Defines the data packet
    #define MAX_DATA_PACKET 30 
    typedef struct
    {
        PacketHeader Header;
        char Data[MAX_DATA_PACKET];
    }DataPacket;

    DataPacket* pPacketReceived = (DataPacket*)Data;
    
    // Check the start of packet
    if (pPacketReceived->Header.Stx != 0x02)
    {
        // Fail: Missing STX at begin of packet
        return;
    }

    // Check if the packet is for this devive
    if (pPacketReceived->Header.AddressID != MY_ADRESS)
    {
        // Fail: Packet for another device
        return;
    }

    // Check the size of packet
    if (DataSize < pPacketReceived->Header.SizeData + sizeof(PacketHeader) || pPacketReceived->Header.SizeData > MAX_DATA_PACKET)
    {
        // Fail: Incompatible data size
        return;
    }

    // Calculates the sum of packet
    int sum = 0;
    for (int i = 0; i < pPacketReceived->Header.SizeData; i++)
    {
        sum += (int)pPacketReceived->Data[i];
    }

    sum += pPacketReceived->Header.Stx;
    sum += pPacketReceived->Header.AddressID;
    sum += pPacketReceived->Header.CommandID;
    sum += pPacketReceived->Header.SizeData;

    // Check the sum of packet
    if (sum != pPacketReceived->Header.Checksum)
    {
        // Fail: Checksum error
        return;
    }

    // Call the function to process the incomming command. * In a real implementation, is good to postponed this calling to return faster from this interrupt routine
    ProcessIncommingCommand(pPacketReceived->Header.CommandID, pPacketReceived->Data, pPacketReceived->Header.SizeData);
}


void ProcessIncommingCommand(char Command, char* Data, int DataSize)
{
    // Some example of commands 
    #define READ_SENSOR 0x05 
    #define WRITE_ACTUATOR 0x06
       
    switch (Command)
    {
        case READ_SENSOR:
        {
            // ProcessReadData();
            
            // Build a new response packet and send back to Master over the UART (tied to some RS485-like driver)
            // SendResponsetoMaster();  

            break;
        }

        case WRITE_ACTUATOR:
        {
            // ProcessWriteData(Data,DataSize);

            // Build a new response packet and send back to Master over the UART (tied to some RS485-like driver)
            // SendResponsetoMaster();  

            break;
        }      
    }
}
// Question 4 section code - End


