//
// Created by Gudleyd on 2018-12-29.
//

#ifndef ULTIMATEINT_ULTIMATEINT_H
#define ULTIMATEINT_ULTIMATEINT_H

#include <vector>
#include <string>
#include <iostream>

namespace gdl {

    class UltimateInt{

    public:

        UltimateInt();
        explicit UltimateInt(const int &);
        explicit UltimateInt(const char*);
        explicit UltimateInt(const std::string &);


        UltimateInt& operator+=(const UltimateInt&);
        UltimateInt& operator-=(const UltimateInt&);
        UltimateInt& operator*=(const UltimateInt&);
        UltimateInt& operator/=(const UltimateInt&);
        UltimateInt& operator%=(const UltimateInt&);

        UltimateInt& operator++();
        UltimateInt& operator--();

        const UltimateInt operator++(int);
        const UltimateInt operator--(int);

        UltimateInt& operator+();
        UltimateInt& operator-();

        UltimateInt& operator=(const UltimateInt&);
        UltimateInt& operator=(const std::string&);

        friend UltimateInt pow(const UltimateInt&, const UltimateInt&);
        friend UltimateInt pow(const UltimateInt&, const int);

        friend std::istream& operator>>(std::istream&, UltimateInt&);
        friend std::ostream& operator<<(std::ostream&, const UltimateInt&);

        friend UltimateInt operator+(const UltimateInt&, const UltimateInt&);
        friend UltimateInt operator-(const UltimateInt&, const UltimateInt&);
        friend UltimateInt operator*(const UltimateInt&, const UltimateInt&);
        friend UltimateInt operator/(const UltimateInt&, const UltimateInt&);
        friend UltimateInt operator%(const UltimateInt&, const UltimateInt&);

        friend bool operator>(const UltimateInt&, const UltimateInt&);
        friend bool operator>=(const UltimateInt&, const UltimateInt&);
        friend bool operator<(const UltimateInt&, const UltimateInt&);
        friend bool operator<=(const UltimateInt&, const UltimateInt&);

        friend bool operator==(const UltimateInt&, const UltimateInt&);
        friend bool operator!=(const UltimateInt&, const UltimateInt&);

        static UltimateInt abs(const UltimateInt&);
        
        int8_t sign();

    private:

        std::vector<int8_t> _num;
        signed char _sign;

        void crop();

        static UltimateInt ONE;
    };

}


#endif //ULTIMATEINT_ULTIMATEINT_H
