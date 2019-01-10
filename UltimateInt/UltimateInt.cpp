#include"UltimateInt.h"

namespace gdl {

    UltimateInt UltimateInt::ZERO = UltimateInt(0);
    UltimateInt UltimateInt::ONE = UltimateInt(1);
    UltimateInt UltimateInt::TWO = UltimateInt(2);

    UltimateInt::UltimateInt() {
        this->clear();
    }

    UltimateInt::UltimateInt(const int& _val) {
        *this = UltimateInt((int64_t)_val);
    }

    UltimateInt::UltimateInt(const int64_t& _val) {
        int64_t val = _val;
        this->_num.clear();
        if (val == 0) {
            this->_sign = 0;
        } else {
            this->_sign = 1;
            if (val < 0) {
                val *= -1;
                this->_sign = -1;
            }
            while (val != 0) {
                this->_num.emplace_back(val % BASE);
                val /= BASE;
            }
        }
        this->crop();
    }

    UltimateInt::UltimateInt(const char* _val) {
        this->_num.clear();
        this->_sign = 1;
        int64_t st = 0;
        if (_val[0] == '-' || _val[0] == '+') {
            this->_sign = (_val[0] == '-' ? (unsigned char)-1 : (unsigned char)1);
            st++;
        }
        auto _sz = (int64_t)std::strlen(_val);
        if (st && (_sz == 1)) {
            return;
        }
        while (true) {
            if (_sz <= st) {
                break;
            }
            int64_t _ln = 0;
            int dig = 0;
            int64_t prefix = 1;
            for (int64_t i = _sz - 1; i >= st && i >= _sz - BASE_LENGTH - st; --i) {
                dig += (_val[i] - '0') * prefix;
                prefix *= 10;
                ++_ln;
            }
            this->_num.emplace_back(dig);
            _sz -= _ln;
        }
        this->crop();
    }

    UltimateInt::UltimateInt(const std::string& _val) {
        this->_num.clear();
        this->_sign = 1;
        int64_t st = 0;
        if (_val[0] == '-' || _val[0] == '+') {
            this->_sign = (_val[0] == '-' ? (unsigned char)-1 : (unsigned char)1);
            st++;
        }
        auto _sz = (int64_t)_val.size();
        if (st && (_sz == 1)) {
            return;
        }
        while (true) {
            if (_sz <= st) {
                break;
            }
            int64_t _ln = 0;
            int dig = 0;
            int64_t prefix = 1;
            for (int64_t i = _sz - 1; i >= st && i >= _sz - BASE_LENGTH; --i) {
                dig += (_val[i] - '0') * prefix;
                prefix *= 10;
                ++_ln;
            }
            this->_num.emplace_back(dig);
            _sz -= _ln;
        }
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

    UltimateInt& UltimateInt::operator+() {
        return *this;
    }

    UltimateInt& UltimateInt::operator-() {
        auto *_r = new UltimateInt;
        *_r = *this;
        _r->_sign *= -1;
        return *_r;
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
            int64_t buf = 0;
            for (size_t i = 0; i < std::max(t_length, n_length); i++) {
                int64_t sum = 0;
                if (i >= t_length) {
                    sum = _n._num[i];
                } else if (i >= n_length) {
                    sum = this->_num[i];
                } else {
                    sum = _n._num[i] + this->_num[i];
                }
                sum += buf;
                buf = sum / (int64_t)BASE;
                _r._num.emplace_back(sum % BASE);
            }
            while (buf != 0) {
                _r._num.emplace_back(buf % BASE);
                buf /= BASE;
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
                        this->_num[i] += BASE;
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
        } else if (this->_sign == _n._sign && this->_sign == -1) {
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
            *this = ZERO;
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
            buf = _r[i] / BASE;
            _r[i] %= BASE;
        }
        _r.push_back(buf);
        this->_num = std::vector<int64_t> (_r.size());
        for (size_t i = 0; i < _r.size(); i++) {
            this->_num[i] = (int64_t)_r[i];
        }
        this->crop();
        return *this;
    }

    UltimateInt operator/(const UltimateInt& _ui1, const UltimateInt& _ui2) {
        UltimateInt _r(_ui1);
        _r /= _ui2;
        return _r;
    }

    int64_t UltimateInt::bin_division(const UltimateInt& _n1, const UltimateInt& _n2) {
        if (_n1._num.size() == 1 && _n2._num.size() == 1) {
            return _n1._num[0] / _n2._num[0];
        }
        if (_n1._num.size() == 2 && _n2._num.size() == 1) {
            return (_n1._num[0] + _n1._num[1] * BASE) / _n2._num[0];
        }
        int64_t _l = 0, _r = BASE;
        while (_r - _l > 1) {
            int64_t _mm = (_r + _l) / 2;
            UltimateInt _m = UltimateInt(_mm);
            UltimateInt _res = _n2 * _m;
            if (_res == _n1) {
                return _mm;
            }
            if (_res < _n1) {
                _l = _mm;
            } else {
                _r = _mm;
            }
        }
        return _l;
    }


    std::pair<UltimateInt, UltimateInt> UltimateInt::division(const UltimateInt& _n1, const UltimateInt& _n) {
        if (_n._sign == 0) {
            return std::make_pair(UltimateInt(0), UltimateInt(0));
        }
        if (_n1._num.size() < _n._num.size()) {
            return std::make_pair(UltimateInt(0), _n1);
        }
        if (_n1._num.size() == 1) {
            return std::make_pair(UltimateInt((_n1._sign * _n1._num[0]) / _n._num[0]), UltimateInt((_n1._sign * _n1._num[0]) % _n._num[0]));
        }
        UltimateInt _q, _r;
        _q._sign = _n1._sign * _n._sign;
        UltimateInt TEN(BASE);
        UltimateInt _d = UltimateInt::abs(_n);
        for (size_t i = _n1._num.size(); i >= 1; i--) {
            _r *= TEN;
            if (_r._num.empty()) {
                _r._num.emplace_back(0);
                _r._sign = 1;
            }
            _r._num[0] = _n1._num[i - 1];
            int64_t cnt = UltimateInt::bin_division(_r, _d);
            _r -= _d * UltimateInt(cnt);
            while (_q._num.size() < i) {
                _q._num.emplace_back(0);
            }
            _q._num[i - 1] = cnt;
        }
        _r._sign = _n1._sign;
        _q.crop();
        _r.crop();
        return std::make_pair(_q, _r);
    }

    UltimateInt& UltimateInt::operator/=(const UltimateInt& _n) {
        *this = division(*this, _n).first;
        return *this;
    }

    UltimateInt operator%(const UltimateInt& _ui1, const UltimateInt& _ui2) {
        UltimateInt _r(_ui1);
        _r %= _ui2;
        return _r;
    }

    UltimateInt& UltimateInt::operator%=(const UltimateInt& _n) {
        *this = division(*this, _n).second;
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

    std::string __to_str(int64_t _n, bool z = 1) {
        std::string s;
        while (_n != 0) {
            s += static_cast<char>(_n % 10) + '0';
            _n /= 10;
        }
        std::reverse(s.begin(), s.end());
        return (z ? std::string(BASE_LENGTH - s.size(), '0') : "") + s;
    }

    int UltimateInt::operator[](const int& _n) {
        if (this->is_null()) {
            if (_n == 0) {
                return 0;
            }
            return -1;
        }
        int64_t i = _n / BASE_LENGTH;
        int64_t j = _n % BASE_LENGTH;
        return __to_str(this->_num[i])[BASE_LENGTH - 1 - j] -'0';
    }

    std::string UltimateInt::to_string(const UltimateInt& _n) {
        if (_n._sign == 0) {
            return "0";
        }
        std::string _res;
        if (_n._sign == -1) {
            _res += '-';
        }
        for (size_t i = _n._num.size() - 1; i > 0; i--) {
            _res += __to_str(_n._num[i], i != _n._num.size() - 1);
        }
        _res += __to_str(_n._num[0], _n._num.size() != 1);
        return _res;
    }

    int64_t UltimateInt::size() {
        if (this->is_null()) {
            return 1;
        }
        return static_cast<int64_t>(this->_num.size() - 1) * BASE_LENGTH + __to_str(this->_num[this->_num.size() - 1], 0).size();
    }

    std::ostream& operator<<(std::ostream& out, const UltimateInt& _n) {
        out << UltimateInt::to_string(_n);
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
        _r._sign = _r._sign * _r._sign;
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

    bool UltimateInt::is_null() {
        return !this->_sign;
    }

    UltimateInt UltimateInt::pow(const UltimateInt& _n) {
        UltimateInt p = _n;
        UltimateInt res = ONE, a = *this;
        while (p._sign != 0) {
            if (p._num[0] & 1) {
                res *= a;
            }
            a *= a;
            p /= TWO;
        }
        return res;
    }

    UltimateInt UltimateInt::pow(const int& _n) {
        int p = _n;
        UltimateInt res = ONE, a = *this;
        while (p > 0) {
            if (p & 1) {
                res *= a;
            }
            if (p >>= 1) {
                a *= a;
            }
        }
        return res;
    }

    std::vector<int> UltimateInt::to_binary(const int &sz) {
        UltimateInt _c = *this;
        std::vector <int> _res;
        while (!_c.is_null()) {
            _res.push_back(_c._num[0] % 2);
            _c /= TWO;
        }
        while (_res.size() < sz) {
            _res.push_back(0);
        }
        size_t ln = _res.size();
        for (size_t i = 0; i < ln / 2; i++) {
            std::swap(_res[i], _res[ln - 1 - i]);
        }
        if (_res.empty()) {
            _res.push_back(0);
        }
        return _res;
    }

    void UltimateInt::from_binary(const std::vector<int>& _v) {
        size_t len = _v.size();
        for (size_t i = 0; i < len; i++) {
            *this *= TWO;
            if (_v[i]) {
                *this += ONE;
            }
        }
        this->crop();
    }

    void UltimateInt::from_binary(const std::string& _v) {
        size_t len = _v.length();
        for (size_t i = 0; i < len; i++) {
            *this *= TWO;
            if (_v[i] - '0') {
                *this += ONE;
            }
        }
        this->crop();
    }

    void UltimateInt::clear() {
        this->_num.clear();
        this->_sign = 0;
    }

    UltimateInt UltimateInt::ui_random(size_t size) {
        UltimateInt buf(1), res;
        for (size_t i = 0; i < size; ++i) {
            int rnd = rand() % 2;
            if (rnd) {
                res += buf;
            }
            buf *= UltimateInt::TWO;
        }
        return res;
    }
}