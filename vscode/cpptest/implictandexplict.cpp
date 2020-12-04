#include<iostream>
class test{
    public:
        test(std::string s){
            value=s;
        }
        void print(){
            std::cout<<value<<std::endl;
        }
    private:
        std::string value;
        static int static_value;
};
int test::static_value=8;
void print_test(test dt){
    dt.print();
}
int main(){
    test nn("who are you");
    nn.print();
    print_test(std::string("i am fine"));
    return 0;
}