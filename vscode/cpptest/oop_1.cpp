#include<iostream>
#include<string>
class Quote{
public:
    Quote()=default;
    Quote(const std::string &book,double sales_price):bookNo(book),price(sales_price){}
    virtual ~Quote()=default;
    std::string isbn()const{return bookNo;};
    virtual double net_price(std::size_t n)const{return n*price;};
private:
    std::string bookNo;//has its own default constructor so Quote constructor call its default constructor to construct this object
protected:
    double price;//can be different to other class
};

double print_total(std::ostream& os,const Quote & item,std::size_t n){
    double ret =item.net_price(n);
    os<<"isbn: "<<item.isbn()<<"total due: "<<ret<<std::endl;
    return ret;
}

int main(int argc,char ** argv){
    Quote obj("JJ",2);
    std::cout<<obj.isbn();
    print_total(std::cout,obj,3);
}