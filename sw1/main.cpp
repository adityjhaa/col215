#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

using namespace std;


int maxvalue(map<string,float> mp){
    float m=0;
    for(auto i : mp){
        m = max(m, i.second);
    }
    return m;
}


bool mapequality(map<string,float> a, map<string,float> b){
    for(auto i : b){
        if(i.second!=a[i.first]){
            return false;
        }
    }
    return true;
}

int main(int argc, char *argv[]){

    char part = argv[1][0];

    map<string,float> gate_del;
        
    ifstream inFile3(argv[3]);
    string line3;
    while(getline(inFile3,line3)){
        istringstream iss(line3);
        string type;
        float element;
        iss >> type;
        iss >> element;
        gate_del[type]=float(element);
    }
    inFile3.close();

    

    if(part=='A'){
        map<string,float> delays;
        vector<string> primaryInputs;
        vector<string> primaryOutputs;
        vector<string> internalSignals;
        ifstream inFile(argv[2]);

        if (!inFile.is_open()) {
            cerr << "Failed to open input file." << endl;
            return 1;
        }

        string line;

        while (getline(inFile, line)) {
            if (line.empty() || line.find("//")==1) {
                continue;
            }
            istringstream iss(line);
            string type, element;
            iss >> type;

            if(type=="PRIMARY_INPUTS"){
                while(iss >> element){
                    primaryInputs.push_back(element);
                }
            } 
            else if(type=="PRIMARY_OUTPUTS"){
                while(iss >> element){
                    primaryOutputs.push_back(element);
                }
            } 
            else if(type=="INTERNAL_SIGNALS"){
                while (iss >> element){
                    internalSignals.push_back(element);
                    }
            }
        }
        for(auto a : primaryInputs){
            delays[a]=0;
        }
        
        inFile.close();

        ifstream inFile2(argv[2]);

        if (!inFile2.is_open()) {
            cerr << "Failed to open input file." << endl;
            return 0;
        }
        string line1;
        while (getline(inFile2, line1)) {
            if (line1.empty() || line1.find("//")==1|| line1.find("PRIMARY_INPUTS")==1 || line1.find("PRIMARY_)OUTPUTS")==1 || line1.find("INTERNAL_SIGNALS")==1 ){
                continue;
            }
            istringstream iss(line1);
            string type, element;
            iss >> type;
            if(type=="INV"){
                iss >>element;
                string t = element;
                iss >> element;
                delays[element]=delays[t]+gate_del["INV"];
            }
            if(type=="AND2"){
                iss >>element;
                string t = element;
                iss >> element;
                string t2 =element;
                iss>>element;
                delays[element]=max(delays[t],delays[t2]) +gate_del["AND2"];
            }
            if(type=="NAND2"){
                iss >>element;
                string t = element;
                iss >> element;
                string t2 =element;
                iss>>element;
                delays[element]=max(delays[t],delays[t2]) +gate_del["NAND2"];
            }
            if(type=="OR2"){
                iss >>element;
                string t = element;
                iss >> element;
                string t2 =element;
                iss>>element;
                delays[element]=max(delays[t],delays[t2]) +gate_del["OR2"];
            }
            if(type=="NOR2"){
                iss >>element;
                string t = element;
                iss >> element;
                string t2 =element;
                iss>>element;
                delays[element]=max(delays[t],delays[t2]) +gate_del["NOR2"];
            }
        }

        ofstream outfile("output_delays.txt");
            for (auto a : primaryOutputs){
                outfile << a << " "<< int(delays[a])<<endl;
            }
        
        outfile.close();


    }else if(part=='B'){
        map<string,float> delays;
        vector<string> primaryInputs;
        vector<string> primaryOutputs;
        vector<string> internalSignals;
        ifstream inFile(argv[2]);

        string line;

        while (getline(inFile, line)) {
            if (line.empty() || line.find("//")==1) {
                continue;
            }
            istringstream iss(line);
            string type, element;
            iss >> type;

            if(type=="PRIMARY_INPUTS"){
                while(iss >> element){
                    primaryInputs.push_back(element);
                }
            } 
            else if(type=="PRIMARY_OUTPUTS"){
                while(iss >> element){
                    primaryOutputs.push_back(element);
                }
            } 
            else if(type=="INTERNAL_SIGNALS"){
                while (iss >> element){
                    internalSignals.push_back(element);
                    }
            }
        }
        for(auto a : primaryInputs){
            delays[a]=0;
        }
        for(auto a : primaryOutputs){
            delays[a]=-1;
        }
        inFile.close();

        map<string,float> req_del;
        
        ifstream inFile2(argv[4]);
        string line2;
        while(getline(inFile2,line2)){
            istringstream iss(line2);
            string type;
            float element;
            iss >> type;
            iss >> element;
            req_del[type]=float(element);
        }
        inFile2.close();

        for(delays[primaryInputs[1]]=maxvalue(req_del); delays[primaryInputs[1]]>=0;delays[primaryInputs[1]]--){
            for(delays[primaryInputs[0]]=maxvalue(req_del); delays[primaryInputs[0]]>=0;delays[primaryInputs[0]]--){
                ifstream inFile1(argv[2]);
                                        
                string line1;

                while (getline(inFile1, line1)) {
                    if (line1.empty() || line1.find("//")==1|| line1.find("PRIMARY_INPUTS")==1 || line1.find("PRIMARY_)OUTPUTS")==1 || line1.find("INTERNAL_SIGNALS")==1 ){
                        continue;
                    }
                    istringstream iss(line1);
                    string type, element;
                    iss >> type;
                    if(type=="INV"){
                        iss >>element;
                        string t = element;
                        iss >> element;
                        delays[element]=delays[t]+2;
                    }
                    if(type=="AND2"){
                        iss >>element;
                        string t = element;
                        iss >> element;
                        string t2 =element;
                        iss>>element;
                        delays[element]=max(delays[t],delays[t2]) +4;
                    }
                    if(type=="NAND2"){
                        iss >>element;
                        string t = element;
                        iss >> element;
                        string t2 =element;
                        iss>>element;
                        delays[element]=max(delays[t],delays[t2]) +3;
                    }
                    if(type=="OR2"){
                        iss >>element;
                        string t = element;
                        iss >> element;
                        string t2 =element;
                        iss>>element;
                        delays[element]=max(delays[t],delays[t2]) +4.5;
                    }
                    if(type=="NOR2"){
                        iss >>element;
                        string t = element;
                        iss >> element;
                        string t2 =element;
                        iss>>element;
                        delays[element]=max(delays[t],delays[t2]) +3.5;
                    }
                }
                inFile1.close();
                if(mapequality(delays,req_del)){
                    break;
                }
            }
            if(mapequality(delays,req_del)){
                break;
            }
        }
        
        

        ofstream outfile("input_delays.txt");
            if(!mapequality(delays,req_del)){
                outfile <<"These are not possible delays for outputs."<<endl;
            }else{
                for (auto a : primaryInputs){
                    outfile << a << " "<< delays[a]<<endl;
                }
            }
        outfile.close();
    }
    

    return 0;
}


