#include <iostream>
#include <string>
#include <vector>
#include <memory>


enum class code_type { 
    code128 = 1,
    dmc
};

class iReadFabNr {
public:
    virtual ~iReadFabNr() = default;
    virtual std::string get_Fab_Nr() = 0;
};

class iReadMatSerialNo {
public:
    virtual ~iReadMatSerialNo() = default;
    virtual std::string get_Mat_Nr() = 0;
    virtual std::string get_Serial_no() = 0;
};


class code128 : public iReadFabNr {
public:
    code128(const std::string &raw_data): m_raw_data(raw_data.begin(), raw_data.end()){}
    std::string get_Fab_Nr() override {
        //Werte die m_raw_data entsprechend aus
    }
private:
    std::vector<unsigned char> m_raw_data;
};

class dmc : public iReadMatSerialNo {
public:
    dmc(const std::string &raw_data): m_raw_data(raw_data.begin(), raw_data.end()){}
    std::string get_Mat_Nr() override {
        //Werte die m_raw_data entsprechend aus
    }
    std::string get_Serial_no() override {
        //Werte die m_raw_data entsprechend aus
    }
private:
    std::vector<unsigned char> m_raw_data;
};

// Factory Template
template<typename T>
class iCode_factory {
public:
    virtual std::unique_ptr<T> CreateReadCode(code_type type, const std::string &raw_data){
        if(type == code_type::code128){
            return std::make_unique<code128>(raw_data);
        }          
        if (type == code_type::dmc){
            return std::make_unique<dmc>(raw_data);
        }
    }
};

typedef struct
{
  uint32_t baudrate;
  uint16_t parity; 
  uint16_t data_bits; 
  uint16_t stop_bits;
  uint16_t flow_control; 

}serial_rs232_parameter;

// Serielles Interface nur lesend
class aSerial_RS232_RD {
public:
    virtual ~aSerial_RS232_RD() = default;
    virtual bool connect() = 0;
    virtual bool disconnect() = 0;
    virtual int32_t read(void *buffer) = 0;
protected:
    uint8_t m_COM;
    serial_rs232_parameter m_settings;
};

class Scanner : aSerial_RS232_RD {
public:
    Scanner(uint8_t &COM,  serial_rs232_parameter &settings)
    {
        this->m_COM = COM;
        this->m_settings = settings;
    }
    
    ~Scanner(){
        disconnect();
    }

    template<typename T>
    std::unique_ptr<T> Scan(code_type type, iCode_factory<T>& factory) {
        if(!connect()){
            return nullptr;
        }
        char code_data[256] = {0};

        if(read(&code_data) <= 0){
            return nullptr;
        }
        else{
            return factory.CreateReadCode(type, code_data);
        }
        return nullptr;  
    }
private:

    bool connect() override {
        // connect to com-port with settings
    }
    bool disconnect() override {
        // disconnect from com-port
    }
    int32_t read(void *buffer)override {
        // read data from com-port
    }

};
