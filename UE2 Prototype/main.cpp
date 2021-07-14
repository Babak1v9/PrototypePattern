#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>

enum Type {
    Sword_long,
    Sword_short
};

class Sword {
protected:
    std::string Sword_name_;
    float Sword_field_;

public:
    Sword() {}
    Sword(std::string Sword_name)
            : Sword_name_(Sword_name) {
    }
    virtual ~Sword() {}
    virtual Sword *Clone() const = 0;
    virtual void Method(float Sword_field) {
        this->Sword_field_ = Sword_field;
        std::cout << "Call Method from " << Sword_name_ << " with field : " << Sword_field << std::endl;
    }
};

class LongSword : public Sword {
private:
    float concrete_Sword_field1_;

    ~LongSword() {
        std::cout << "LongSword destroyed." << std::endl;
    }

public:
    LongSword(std::string Sword_name, float concrete_Sword_field)
            : Sword(Sword_name), concrete_Sword_field1_(concrete_Sword_field) {
        std::cout << "LongSword was created.\n";
    }

    /**
     * Notice that Clone method return a Pointer to a new LongSword
     * replica. so, the client (who call the clone method) has the responsability
     * to free that memory. I you have smart pointer knowledge you may prefer to
     * use unique_pointer here.
     */
    Sword *Clone() const override {
        return new LongSword(*this);
    }
};

class ShortSword : public Sword {
private:
    float concrete_Sword_field2_;

    ~ShortSword() {
        std::cout << "ShortSword destroyed." << std::endl;
    }


public:
    ShortSword(std::string Sword_name, float concrete_Sword_field)
            : Sword(Sword_name), concrete_Sword_field2_(concrete_Sword_field) {
        std::cout << "ShortSword was created.\n";
    }
    Sword *Clone() const override {
        return new ShortSword(*this);
    }
};

class PrototypeManager {
public:
    static PrototypeManager& instance() {
        static PrototypeManager _instance;
        return _instance;
    }
    ~PrototypeManager() {
        std::cout << "\nDestroyed Prototype Manager.\n";
    }

    std::vector<std::string> readConfig(std::string filename) {

        std::ifstream file;
        std::string type;
        std::vector<std::string> s;

        file.open(filename);
        if (!file) {
            std::cout << "Unable to open File";
            exit(1);
        }

        while(std::getline(file, type)) {
            if (type != " ") {
                s.push_back(type);
            }

        }

        for (const auto& i : s) {
            std::cout << i << "\n";
        }

         return s;
    }

    //get types from config file and instantiate prototype via Enum, Name or Int
    void getTypes() {

    }

    void instantiatePrototype(std::string type) {

       std::cout  <<std::endl << type << std::endl;

        if (type.compare("LongSword") == 0) {

            std::cout << "Let's create a LongSword.\n";
            std::unique_ptr<Sword> sword(new LongSword("Sword_long", 50.f));
            sword->Method(90);

        } else if (type.compare("ShortSword") == 0) {

            std::cout << "Let's create a ShortSword.\n";
            std::unique_ptr<Sword> sword(new ShortSword("Sword_short", 60.f));
            sword->Method(10);
        } else {
            std::cout << "Object Type does not exist, no object was created.";
        }
    }

private:
    PrototypeManager() {
        std::cout << "Starting PrototypeManager...\n\n";
    }
    PrototypeManager(const PrototypeManager&); //prevents creating new instances via copy constructor
    PrototypeManager& operator = (const PrototypeManager&); //prevents further instances through copies
};

int main() {

    std::vector<std::string> types;

    PrototypeManager& pm = PrototypeManager::instance();

    types = pm.readConfig("config.txt");

    for (const auto& i : types) {
        pm.instantiatePrototype(i);
    }

    return 0;
}
