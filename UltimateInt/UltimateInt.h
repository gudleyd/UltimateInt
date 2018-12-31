//
// Created by Gudleyd on 2018-12-29.
//

#ifndef ULTIMATEINT_ULTIMATEINT_H
#define ULTIMATEINT_ULTIMATEINT_H

#include <vector>
#include <string>
#include <iostream>

#define NUMBER_TYPE int64_t

namespace gdl {

    class UltimateInt{

    public:

        UltimateInt();
        explicit UltimateInt(const int&);
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

        UltimateInt pow(const UltimateInt&);
        UltimateInt pow(const int&);

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
        bool is_null();

        std::vector<int8_t> binary(int sz = 0);

    private:

        std::vector<NUMBER_TYPE> _num;
        signed char _sign;

        void crop();

        static UltimateInt ZERO;
        static UltimateInt ONE;
        static UltimateInt TWO;

        std::pair<UltimateInt, UltimateInt> division(const UltimateInt&, const UltimateInt&);

    };

}


#endif //ULTIMATEINT_ULTIMATEINT_H
