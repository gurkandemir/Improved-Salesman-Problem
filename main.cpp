#include <iostream>
#include "vector"
#include <sstream>
#include <algorithm>
#include <iterator>
#include "iomanip"
#include "fstream"
#include <queue>
using namespace std;

//global declerations
#define INF 0x3f3f3f3f
typedef pair<int, int> iPair;
int dist[201][1<<(14)];
iPair parent[201][1<<(14)];
int thieves[201][201];
vector<iPair> adj[201];
int collection[201];

//class which is useful for priority queue ordering
class priority{
public:
    int dist;
    int city;
    int key;

    priority(int d,int c,int k){
        dist=d;
        city=c;
        key=k;
    }


};

//method in order to choose the smallest distance in queue
struct CustomCompare{
    bool operator()(const priority& lhs, const priority& rhs) {
        return lhs.dist > rhs.dist;
    }
};

template <class Container>
void split1(const string& str, Container& cont)
{
    istringstream iss(str);
    copy(istream_iterator<string>(iss),
         istream_iterator<string>(),
         back_inserter(cont));
}


int main(int argc, char* argv[]) {
    //reads input
    fstream infile(argv[1]);
    string line = "";
    vector<string> words;
    getline(infile, line);
    split1(line, words);
    int n=stoi(words[0]),m=stoi(words[1]),p=stoi(words[2]),k=stoi(words[3]);
    for(int i=0;i<n+1;i++)
        collection[i]=0;

    for(int i=0;i<k;i++){
        int temp=0;
        vector<string> words;
        getline(infile, line);
        split1(line, words);
        for(int j=1;j<=stoi(words[1]);j++) {
            temp|=(1 << stoi(words[j + 1]));
        }

        collection[stoi(words[0])]|=temp;
    }

    for(int i=0;i<n+1;i++){
        for(int j=0;j<n+1;j++)
            thieves[i][j]=0;
    }

    for(int i=0;i<m;i++){
        int temp=0;
        vector<string> words;
        getline(infile, line);
        split1(line, words);
        adj[stoi(words[0])].push_back(make_pair(stoi(words[1]),stoi(words[2])));
        adj[stoi(words[1])].push_back(make_pair(stoi(words[0]),stoi(words[2])));

        for(int j=1;j<=stoi(words[3]);j++){
            temp|= (1<< stoi(words[j+3]));
        }

        thieves[stoi(words[0])][stoi(words[1])]=temp;
        thieves[stoi(words[1])][stoi(words[0])]=temp;
    }



    for(int i=0;i<(n+1);i++){
        for(int j=0;j<(1<<(p+1));j++){
            dist[i][j]=INF;
            parent[i][j].first=INF;
            parent[i][j].second=INF;
        }
    }

    priority_queue<priority,vector<priority>, CustomCompare> pq;
    pq.push(priority(0,1,collection[1]));
    int cou=0;
    int k1=0;

    //finds the shortest path.
    while(!pq.empty()){
    	priority temp=pq.top();
    	pq.pop();
        int curr_node=temp.city;
        //if it is n, it terminates in other words it finds the shortest path.
        if(curr_node!=n) {
            dist[curr_node][temp.key] = temp.dist;
            int keys = 0;
            vector<iPair>::iterator itr;
            keys = temp.key | collection[curr_node];

            //visits neighbours.
            for (itr = adj[curr_node].begin(); itr != adj[curr_node].end(); itr++) {
                iPair pair = *itr;
                int curr_neighbour = pair.first;
                int curr_weight = pair.second;
                int check = thieves[curr_node][curr_neighbour] & keys;

                //checks whether road is valid or not.
                if (check == thieves[curr_node][curr_neighbour]) {
                    if (dist[curr_neighbour][keys] > dist[curr_node][temp.key] + curr_weight) {
                        dist[curr_neighbour][keys] = dist[curr_node][temp.key] + curr_weight;
                        if(curr_neighbour==n)
                            cou++;

                        parent[curr_neighbour][keys].first=curr_node;
                        parent[curr_neighbour][keys].second=temp.key;
                        pq.push(priority(dist[curr_neighbour][keys], curr_neighbour, keys));
                    }
                }

            }
        }
        else {
            k1=temp.key;
            break;
        }
    }

    //writes output.
    ofstream myfile;
    myfile.open(argv[2]);

    //if there is no path
    if(cou==0)
        myfile<<-1;

    //recursively writes path
    else{
        string s;
        s=to_string(n);
        int cur=n;
        while(true){
            iPair temp=parent[cur][k1];
            s=to_string(temp.first)+" "+s;
            cur=temp.first;
            k1=temp.second;

            if(cur==1 && k1==collection[1])
                break;
        }

        myfile<<s;

    }

    myfile.close();

    return 0;
}
