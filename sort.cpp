#include <bits/stdc++.h>
using namespace std;
using namespace std::chrono;

template <typename T>   void selection(T s,T e);    // unstable in my code

template <typename T>   void insertion(T s,T e);    // stable

template <typename T>   void merge_sort(T s,T e);   // stable
template <typename T>   void merge_range(T s,T mid,T e);

template <typename T>   void quick(T s,T e);        // unstable
template <typename T>   T part(T s,T e);
template <typename T>   T med_pivot(T s,T m,T e);

template <typename T>   void hybrid(T s,T e);       // unstable
template <typename T>   void hybrid_utility(T s,T e,int depth);

bool open_files(int& type, ifstream& in, ofstream& out, ofstream& time, int argc, char **argv);// open the needed files
bool check(int& type,ifstream& in, ofstream& out,ofstream& time);                             // check if valid input
void read_data(ifstream& in, vector<int>& vec);                                               // read input data from file
void sort_data(vector<int>& vec, int& type);                                                  //sort the data
void write_data(ofstream& out,ofstream& time, vector<int>& vec, int duration);                // output the data on files
void close_files(ifstream& in, ofstream& out, ofstream& time);                                // Close all files

int main(int argc, char **argv)
{
    vector<int>vec;     int type; ifstream in;    ofstream time, out;

    if(!open_files(type, in, out,time, argc, argv)) return 0;   // error occured

    read_data(in,vec);                                          // read the data

    auto start = high_resolution_clock::now();                  // the beginning time for execution
    sort_data(vec,type);                                        // sort the data
    auto stop = high_resolution_clock::now();                   // the ending time for execution
    auto duration= duration_cast<milliseconds>(stop-start);     // Calculate the execution time

    write_data(out,time,vec,duration.count());                  // write the data
    vec.clear();

    close_files(in, out, time);                                 // Close all files
}

bool open_files(int& type, ifstream& in, ofstream& out, ofstream& time, int argc, char **argv)
{
    string in_path, out_path, time_path;

    if(argc<5)
    {   cout<<"number of arguments is invalid"<<endl;   return false;   }           // invalid number of arguments

    in_path=argv[2];    out_path=argv[3];   time_path=argv[4];  type=*argv[1]-'0';  // read arguments
    in.open(in_path);   time.open(time_path);   out.open(out_path);                 // open the needed files

    if(!check(type,in,out,time))
    {   close_files(in, out, time); return false;   }                               // invalid argument

    return true;
}

bool check(int& type,ifstream& in, ofstream& out,ofstream& time)
{
    bool flag=true;

    if(type<0 || type>4)    cout<<"invalid type sort"<<endl,    flag=false;         // invalid type sort
    if(in.fail( ))          cout<<"invalid input path "<<endl,    flag=false;       // invalid input path
    if(out.fail( ))         cout<<"invalid output path "<<endl,   flag=false;       // invalid output path
    if(time.fail( ))        cout<<"invalid running_time path "<<endl,  flag=false;  // invalid running_time path

    return flag;
}

void read_data(ifstream& in, vector<int>& vec)
{
    int a;

    in>>a;
    while(!in.eof())    vec.push_back(a),   in>>a;
}

void sort_data(vector<int>& vec, int& type)
{
    switch(type)
    {
        case 0: selection(vec.begin(),vec.end());   break;
        case 1: insertion(vec.begin(),vec.end());   break;
        case 2: merge_sort(vec.begin(),vec.end());  break;
        case 3: quick(vec.begin(),vec.end());       break;
        case 4: hybrid(vec.begin(),vec.end());      break;
    }
}

void write_data(ofstream& out,ofstream& time, vector<int>& vec, int duration)
{
    time<<duration<<endl;
    for(int i=0;i<vec.size();i++)   out<<vec[i]<<endl;
}

void close_files(ifstream& in, ofstream& out, ofstream& time)
{   in.close();     out.close();    time.close();   }

template <typename T>
void selection(T s,T e)
{
    for(T it=s;it!=e;it++)
    {
        T small=it;

        for(T it2=it+1;it2!=e;it2++)                                        // Select smallest element in range [it,e)
            if(*it2<*small) small=it2;

        if(small!=it)   *small+=*it,    *it=*small-*it,     *small-=*it;    // replace small content with it content
    }
}

template <typename T>
void insertion(T s,T e)
{
    typename T::value_type key;

    if(s==e)    return;                 // there is no elements

    for(T it=s+1; it!=e;it++)           // "it" is the position of element need to be inserted
    {
        key = *it;                      // store the value that need to be inserted
        T it2=it-1;                     // "it2" is pointer to traverse on sorted element

        while(it!=s && *it2>key)        // insert element in range [s,it)
            *(it2+1)=*it2,  it2--;      // shift data right

        *(it2+1)=key;                   // insert the value
    }
}

template <typename T>
void merge_sort(T s,T e)
{
    if(e-s<2)   return;     // there are less than two elements

    T mid = s + (e-s)/2;    // get mid
    merge_sort(s,mid);      // call for left sub_array
    merge_sort(mid,e);      // call for right sub_array
    merge_range(s,mid,e);   // merge left sub_array with right sub_array
}
template <typename T>
void merge_range(T s,T mid,T e)
{
    vector<typename T::value_type> l,r;     int i=0,j=0;

    for (T it=s;it!=mid;it++)   l.push_back(*it);               // copy left part in L
    for (T it=mid;it!=e;it++)   r.push_back(*it);               // copy right part in R
    l.push_back(numeric_limits<typename T::value_type>::max()); //push maximum value to L
    r.push_back(numeric_limits<typename T::value_type>::max()); //push maximum value to R

    for(T it=s;it!=e;it++)                                      //merge L and R in the original data
        if(l[i]<=r[j])  *it=l[i],   i++;
        else            *it=r[j],   j++;
}

template <typename T>
void quick(T s,T e)
{
    if(e-s<2)   return; // there are less than two elements

    T q = part(s,e);    // split the array between larger pivot and smaller pivot
    quick(s,q);         // call left sub_array for pivot
    quick(q+1,e);       // call right sub_array for pivot
}
template <typename T>
T part(T s,T e) // assuming pivot is the last element
{
    T temp = s, pivot = med_pivot(s,s + (e-s)/2,e-1);

    if(pivot != (e-1))  // replace pivot with the last element
        *pivot+=*(e-1),    *(e-1)=*pivot-*(e-1),     *pivot-=*(e-1),    pivot = e-1;

    for(T it=s;it!=e;it++) // make the elements smaller than pivot be at left of pivot and the larger be at right of pivot
        if(*it<*pivot)
        {
            if(temp != it)  *temp+=*it,    *it=*temp-*it,     *temp-=*it; // replace temp content with it content
            temp++;
        }
    if(temp != pivot)   *temp+=*pivot,    *pivot=*temp-*pivot,     *temp-=*pivot; // replace temp content with pivot content

    return temp;    // return pivot;
}
template <typename T>
T med_pivot(T s,T m,T e)
{
    if(*s<*e)
    {
        if(*m<*s)   return s;
        if(*e<*m)   return e;

        return m;
    }
    if(*m<*e)   return e;
    if(*s<*m)   return s;

    return m;
}

template <typename T>
void hybrid_utility(T s,T e, int depth)
{
    if(e-s<2)   return;                 // there are less than two elements

    if(e-s<16)
    {   insertion(s,e);     return; }   // if partition size is less than 16 use insertion

    if(depth==0)
    {   merge_sort(s,e);    return; }   // if depth of quick sort larger than 2*log(n) use merge_sort

    T q = part(s,e);                    // split the array between larger pivot and smaller pivot
    hybrid_utility(s,q,depth-1);        // call left sub_array for pivot
    hybrid_utility(q+1,e,depth-1);      // call right sub_array for pivot
}
template <typename T>
void hybrid(T s,T e)
{   hybrid_utility(s,e,2*log(e-s)); }
