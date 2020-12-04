#include<iostream>
#include<string>
#include<vector>
#include<stdlib.h>
class Quote{
public:
    Quote(){
        std::cout<<"base constructor"<<std::endl;
    };
    Quote(const std::string &book,double sales_price):bookNo(book),price(sales_price){}
    virtual ~Quote()=default;
    std::string isbn()const{return bookNo;};
    virtual double net_price(std::size_t n)const{return n*price;};
private:
    std::string bookNo;//has its own default constructor so Quote constructor call its default constructor to construct this object
protected:
    double price;//can be different to other class
};
class Bulk_quote:public Quote{
public:
    Bulk_quote(){
        std::cout<<"derivation class constructor"<<std::endl;

    }
    Bulk_quote(const std::string& book,double p,std::size_t count ,double disc):Quote(book,p),min_bulkcount(count),discount(disc){

    };
    double net_price(std::size_t n)const override{
        if(n>=min_bulkcount)
            return n*price*discount;
        else 
            return n*price;
    };
private:
    std::size_t min_bulkcount=0;
    double discount=0;
    
};

class my_quote:public Quote{
public:
    my_quote(){

    }
    my_quote(const std::string& book,double p,const std::vector<std::size_t>& count,const std::vector<double>& disc):Quote(book,p),dislevel(count),discount(disc){

    }
    double net_price(std::size_t n)const{
        int num=0;
        while(n>=dislevel[num]&&num!=dislevel.size()){
            num++;
        }
        // std::cout<<"callend";
        // std::cout<<"net_price level:"<<num<<std::endl;
        // std::cout<<"net_price discount:"<<discount[num]<<std::endl;
        // std::cout<<"net_price result:"<<price*n*discount[num]<<std::endl;

        return price*n*discount[num];
    }
    double net_price(std::size_t n){
        int num=0;
        while(n>=dislevel[num]&&num!=dislevel.size()){
            num++;
        }
        std::cout<<"net_price level:"<<num<<std::endl;
        std::cout<<"net_price discount:"<<discount[num]<<std::endl;
        std::cout<<"net_price result:"<<price*n*discount[num]<<std::endl;

        return price*n*discount[num];
    }

    void print_policy(){
        std::cout<<"discount level:"<<std::endl;
        for(auto n:dislevel)
            std::cout<<n<<" ";
        std::cout<<"up"<<std::endl;
        for(auto n:discount)
            std::cout<<n<<" ";
        std::cout<<std::endl;

    }
private:
    std::vector<std::size_t> dislevel;
    std::vector<double> discount;
};

double print_total(std::ostream& os,const Quote & item,std::size_t n){
    double ret =item.net_price(n);
    os<<"isbn: "<<item.isbn()<<" total "<<n<<" due: "<<ret<<std::endl;
    return ret;
}

void ss(int n,const my_quote& o){
    o.net_price(n);
    std::cout<<"ss called"<<std::endl;

}
int main(int argc,char ** argv){
    // Quote obj("JJ",2);
    // std::cout<<obj.isbn()<<std::endl;
    // print_total(std::cout,obj,3);
    Bulk_quote objb("JJ",2,2,0.8);
    std::cout<<objb.isbn()<<objb.net_price(4)<<std::endl;
    print_total(std::cout,objb,4);
    std::vector<std::size_t> levels({3,10,20,100});
    std::vector<double> discounts({1,0.95,0.9,0.8,0.6});
    my_quote objm("my_JJ",10,levels,discounts);
    print_total(std::cout,objm,4);
    
    std::size_t num=4;
    // objm.net_price(num);
    ss(num,objm);
    objm.print_policy();
}