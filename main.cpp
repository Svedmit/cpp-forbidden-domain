#include <algorithm>
#include <cassert>
#include <iostream>
#include <fstream>
#include <functional>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

using namespace std;

class Domain {
public:
    Domain(std::string str)
        : domain_('.' + str)
    {
    }

    std::string_view GetValue() const {
        return domain_;
    }

    bool operator==(const Domain& other) const {
        return domain_ == other.domain_;
    }

    bool operator<(const Domain& other) const {
        return lexicographical_compare(
            domain_.rbegin(), domain_.rend(), 
            other.domain_.rbegin(), other.domain_.rend()
        );
    }

    // Возвращает true, если this его поддомен
    bool IsSubdomain(const Domain& other) const {
        return other.domain_.size() <= domain_.size()
            && std::equal(other.domain_.rbegin(), other.domain_.rend(), domain_.rbegin());
    }
private:
    std::string domain_;
};


class DomainChecker {
public:
    template<typename Iterator>
    DomainChecker(Iterator begin, Iterator end)
        : forbidden(begin, end)
    {   
        sort(forbidden.begin(), forbidden.end());
        auto last = unique(forbidden.begin(), forbidden.end(), [](auto a, auto b) {
            return a == b || b.IsSubdomain(a);
        });
        forbidden.erase(last, forbidden.end());
    }

    // Возвращает true, если домен запрещён
    bool IsForbidden(const Domain& domain){
        auto it = upper_bound(forbidden.begin(), forbidden.end(), domain);
        if(it == forbidden.begin()){
            return false;
        }
        --it;
        return *it == domain || domain.IsSubdomain(*it);
    }

private:
    std::vector<Domain> forbidden;
};

template <typename Number>
Number ReadNumberOnLine(istream& input) {
    string line;
    getline(input, line);

    Number num;
    std::istringstream(line) >> num;

    return num;
}

// Читает заданное количество доменов из стандартного входа
std::vector<Domain> ReadDomains(istream& input, size_t domain_count){
    vector<Domain> result;
    result.reserve(domain_count);
    for(size_t i = 0; i < domain_count; ++i){
        string line;
        getline(input, line);
        result.push_back(Domain(line));
    }
    return result;
}

void Tests(){
    const std::vector<Domain> forbidden_domains{
        Domain("gdz.ru"), Domain("maps.me"), Domain("m.gdz.ru"), Domain("com"), 
        Domain("yso.ddtz.tavu.ceb.yin.xihh.ep.diba"), Domain("ceb.yin.xihh.ep.diba"), Domain("k.vw.sak.w.shr.diba"), Domain("vw.sak.w.shr.diba")
    };
    DomainChecker checker(forbidden_domains.begin(), forbidden_domains.end());

    //Сайты из списка
    assert(checker.IsForbidden(Domain("gdz.ru")) == true);
    assert(checker.IsForbidden(Domain("yso.ddtz.tavu.ceb.yin.xihh.ep.diba")) == true);

    //Сайты не из списка
    assert(checker.IsForbidden(Domain("aaa.aa")) == false);
    assert(checker.IsForbidden(Domain("zzz.zz")) == false);

    //Субдомены
    assert(checker.IsForbidden(Domain("aaa.com")) == true);
    assert(checker.IsForbidden(Domain("bla.bla.maps.me")) == true);

    assert(checker.IsForbidden(Domain("co.t.ep.k.vw.sak.w.shr.diba")) == true);
}


int main() {
    //Tests();

    std::ifstream in("input.txt");
    std::cin.rdbuf(in.rdbuf()); //redirect std::cin to in.txt!
    std::ofstream out("out2.txt");

    const std::vector<Domain> forbidden_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    DomainChecker checker(forbidden_domains.begin(), forbidden_domains.end());

    const std::vector<Domain> test_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    for (const Domain& domain : test_domains) {
        out << (checker.IsForbidden(domain) ? "Bad"sv : "Good"sv) << endl;
    }
}