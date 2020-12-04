#include<iostream>
class Screen{
    
    public:
        Screen():conva(1),refer(value) {
            value=0;
            
        }
        void change() const{
            ++value;
        }
        void print(){
            std::cout<<value<<std::endl;
        }
    private:
        mutable int value;
        const int conva;
        int& refer;
};
int main(){
    Screen sc;
    sc.print();
    sc.change();
    sc.print();
}