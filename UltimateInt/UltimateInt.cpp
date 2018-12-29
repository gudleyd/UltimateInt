#include"UltimateInt.h"


namespace gdl {

    UltimateInt UltimateInt::ONE = UltimateInt(1);

    UltimateInt::UltimateInt() {
        this->_num.clear();
        this->_sign = 0;
    }

    UltimateInt::UltimateInt(const int& _val) {
        int val = _val;
        this->_num.clear();
        if (val == 0) {
            this->_num.emplace_back(0);
            this->_sign = 0;
        } else {
            this->_sign = 1;
            if (val < 0) {
                val *= -1;
                this->_sign = -1;
            }
            while (val != 0) {
                this->_num.emplace_back(val % 10);
                val /= 10;
            }
        }
        this->crop();
    }

    UltimateInt::UltimateInt(const char* _val) {
        this->_num.clear();
        this->_sign = 1;
        size_t st = 0;
        if (_val[0] == '-' || _val[0] == '+') {
            this->_sign = (_val[0] == '-' ? (unsigned char)-1 : (unsigned char)1);
            st++;
        }
        size_t length = std::strlen(_val);
        for (size_t i = length - 1; i > st; i--) {
            this->_num.emplace_back(_val[i] - '0');
        }
        this->_num.emplace_back(_val[st] - '0');
        this->crop();
    }

    UltimateInt::UltimateInt(const std::string& _val) {
        this->_num.clear();
        this->_sign = 1;
        size_t st = 0;
        if (_val[0] == '-' || _val[0] == '+') {
            this->_sign = (_val[0] == '-' ? (unsigned char)-1 : (unsigned char)1);
            st++;
        }
        size_t length = _val.size();
        for (size_t i = length - 1; i > st; i--) {
            this->_num.emplace_back(_val[i] - '0');
        }
        this->_num.emplace_back(_val[st] - '0');
        this->crop();
    }

    UltimateInt& UltimateInt::operator++() {
        UltimateInt _sv = *this;
        *this += ONE;
        return *this;
    }

    UltimateInt& UltimateInt::operator--() {
        UltimateInt _sv = *this;
        *this -= ONE;
        return *this;
    }

    const UltimateInt UltimateInt::operator++(int) {
        UltimateInt _sv = *this;
        *this += ONE;
        return _sv;
    }

    const UltimateInt UltimateInt::operator--(int) {
        UltimateInt _sv = *this;
        *this -= ONE;
        return _sv;
    }

    UltimateInt operator+(const gdl::UltimateInt& _ui1, const gdl::UltimateInt& _ui2) {
        UltimateInt _r(_ui1);
        _r += _ui2;
        return _r;
    }

    UltimateInt& UltimateInt::operator+=(const UltimateInt& _n) {
        UltimateInt _r;
        if (this->_sign == 0) {
            _r = _n;
        } else if (_n._sign == 0) {
            return *this;
        } else if (this->_sign == _n._sign) {
            _r._sign = this->_sign;
            size_t t_length = this->_num.size();
            size_t n_length = _n._num.size();
            int8_t buf = 0;
            for (size_t i = 0; i < std::max(t_length, n_length); i++) {
                int8_t sum = 0;
                if (i >= t_length) {
                    sum = _n._num[i];
                } else if (i >= n_length) {
                    sum = this->_num[i];
                } else {
                    sum = _n._num[i] + this->_num[i];
                }
                sum += buf;
                buf = sum / (int8_t)10;
                _r._num.emplace_back(sum % 10);
            }
            while (buf != 0) {
                _r._num.emplace_back(buf % 10);
                buf /= 10;
            }
        } else {
            if (this->_sign == 1) {
                _r = *this - UltimateInt::abs(_n);
            } else {
                _r = _n - UltimateInt::abs(*this);
            }
        }
        _r.crop();
        *this = _r;
        return *this;
    }

    UltimateInt operator-(const UltimateInt& _ui1, const UltimateInt& _ui2) {
        UltimateInt r(_ui1);
        r -= _ui2;
        return r;
    }

    UltimateInt& UltimateInt::operator-=(const UltimateInt& _n) {
        UltimateInt _tmp, _r;
        if (this->_sign == 0) {
            *this = _n;
            this->_sign *= -1;
        } else if (this->_sign == _n._sign && this->_sign == 1) {
            if (*this >= _n) {
                int buf = 0;
                for (size_t i = 0; i < _n._num.size() || buf; ++i) {
                    this->_num[i] -= buf + (i < _n._num.size() ? _n._num[i] : 0);
                    if (this->_num[i] < 0) {
                        this->_num[i] += 10;
                        buf = 1;
                    } else {
                        buf = 0;
                    }
                }
            } else {
                *this = _n - *this;
                this->_sign *= (signed char)-1;
            }
        } else if (this->_sign == 1 && _n._sign == -1) {
            *this = *this + UltimateInt::abs(_n);
        } else if (this->_sign == -1 && _n._sign == 1) {
            *this = UltimateInt::abs(*this) + _n;
            this->_sign *= (signed char)-1;
        } else {
            *this = UltimateInt::abs(_n) - UltimateInt::abs(*this);
        }
        this->crop();
        return *this;
    }

    UltimateInt operator*(const UltimateInt& _ui1, const UltimateInt& _ui2) {
        UltimateInt _r(_ui1);
        _r *= _ui2;
        return _r;
    }

    UltimateInt& UltimateInt::operator*=(const UltimateInt& _n) {
        this->_sign *= _n._sign;
        if (this->_sign == 0) {
            *this = UltimateInt(0);
            return *this;
        }
        std::vector<int64_t> _r;
        for (size_t i = 0; i < this->_num.size(); i++) {
            for (size_t j = 0; j < _n._num.size(); j++) {
                if (i + j < _r.size()) {
                    _r[i + j] += this->_num[i] * _n._num[j];
                } else {
                    _r.push_back(this->_num[i] * _n._num[j]);
                }
            }
        }
        int64_t buf = 0;
        for (size_t i = 0; i < _r.size(); i++) {
            _r[i] += buf;
            buf = _r[i] / 10;
            _r[i] %= 10;
        }
        _r.push_back(buf);
        this->_num = std::vector<int8_t> (_r.size());
        for (size_t i = 0; i < _r.size(); i++) {
            this->_num[i] = (int8_t)_r[i];
        }
        this->crop();
        return *this;
    }

    UltimateInt operator/(const UltimateInt& _ui1, const UltimateInt& _ui2) {
        UltimateInt _r(_ui1);
        _r /= _ui2;
        return _r;
    }

    UltimateInt& UltimateInt::operator/=(const UltimateInt& _n) {
        if (_n._sign == 0) {
            *this = UltimateInt(0);
            return *this;
        }
        if (this->_num.size() < _n._num.size()) {
            *this = UltimateInt(0);
            return *this;
        }
        UltimateInt _q, _r;
        _q._sign = this->_sign * _n._sign;
        UltimateInt TEN(10);
        UltimateInt _d = UltimateInt::abs(_n);
        for (size_t i = this->_num.size(); i >= 1; i--) {
            _r *= TEN;
            if (_r._num.empty()) {
                _r._num.emplace_back(0);
                _r._sign = 1;
            }
            _r._num[0] = this->_num[i - 1];
            int8_t cnt = 0;
            while (_r >= _d) {
                _r -= _d;
                ++cnt;
            }
            while (_q._num.size() < i) {
                _q._num.emplace_back(0);
            }
            _q._num[i - 1] = cnt;
        }
        *this = _q;
        this->crop();
        return *this;
    }

    UltimateInt operator%(const UltimateInt& _ui1, const UltimateInt& _ui2) {
        UltimateInt _r(_ui1);
        _r %= _ui2;
        return _r;
    }

    UltimateInt& UltimateInt::operator%=(const UltimateInt& _n) {
        *this = *this - (*this / _n) * _n;
        return *this;
    }

    UltimateInt& UltimateInt::operator=(const UltimateInt& _n) = default;

    UltimateInt& UltimateInt::operator=(const std::string& _s) {
        *this = UltimateInt(_s);
        return *this;
    }

    std::istream& operator>>(std::istream& in, UltimateInt& _n) {
        std::string s;
        in >> s;
        _n = s;
        return in;
    }

    std::ostream& operator<<(std::ostream& out, const UltimateInt& _n) {
        if (_n._sign == 0) {
            out << 0;
        } else {
            if (_n._sign == -1) {
                out << '-';
            }
            for (size_t i = _n._num.size() - 1; i > 0; i--) {
                out << (int)_n._num[i];
            }
            out << (int)_n._num[0];
        }
        return out;
    }

    bool operator==(const UltimateInt& _ui1, const UltimateInt& _ui2) {
        return (_ui1._sign == _ui2._sign) && (_ui1._num == _ui2._num);
    }

    bool operator!=(const gdl::UltimateInt& _ui1, const gdl::UltimateInt& _ui2) {
        return !(_ui1 == _ui2);
    }

    bool operator>(const UltimateInt& _ui1, const UltimateInt& _ui2) {
        if (_ui1._sign == _ui2._sign) {
            if (_ui1._sign == 0) {
                return false;
            }
            if (_ui1._num.size() != _ui2._num.size()) {
                if (_ui1._sign == 1) {
                    return (_ui1._num.size() > _ui2._num.size());
                } else {
                    return (_ui1._num.size() < _ui2._num.size());
                }
            }
            for (size_t i = _ui1._num.size() - 1; i > 0; i--) {
                if (_ui1._num[i] != _ui2._num[i]) {
                    if (_ui1._sign == 1) {
                        return (_ui1._num[i] > _ui2._num[i]);
                    } else {
                        return (_ui1._num[i] < _ui2._num[i]);
                    }
                }
            }
            if (_ui1._num[0] != _ui2._num[0]) {
                if (_ui1._sign == 1) {
                    return (_ui1._num[0] > _ui2._num[0]);
                } else {
                    return (_ui1._num[0] < _ui2._num[0]);
                }
            }
        }
        return _ui1._sign > _ui2._sign;
    }

    bool operator>=(const UltimateInt& _ui1, const UltimateInt& _ui2) {
        return _ui1 > _ui2 || _ui1 == _ui2;
    }

    bool operator<(const UltimateInt& _ui1, const UltimateInt& _ui2) {
        return !(_ui1 >= _ui2);
    }

    bool operator<=(const UltimateInt& _ui1, const UltimateInt& _ui2) {
        return !(_ui1 > _ui2);
    }

    UltimateInt UltimateInt::abs(const gdl::UltimateInt& _ui) {
        UltimateInt _r(_ui);
        _r._sign = std::abs(_r._sign);
        return _r;
    }

    void UltimateInt::crop() {
        while (!this->_num.empty() && this->_num[this->_num.size() - 1] == 0) {
            this->_num.pop_back();
        }
        if (this->_num.empty()) {
            this->_sign = 0;
        }
    }

    int8_t UltimateInt::sign() {
        return this->_sign;
    }
}