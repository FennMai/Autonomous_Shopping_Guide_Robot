#include <iostream>

using namespace std;

class test{
public:

    test(int weight){
        cout<<"structure int"<< weight << endl;
    };

    test(double weight){
        cout<<"structure double"<< weight<< endl;
    };

    ~test(){
        cout<<"cancel"<<endl;
    };
};


int main()
{
    test t1(50);
    test t2(5.2);// 重载--根据参数的类型不同，可以定义调用不同的构造函数

//    test *t2 = new test; // 堆栈实例化对象
//    delete t2; // 手动释放


    cout << "Hello World!" << endl;
    return 0;
}
