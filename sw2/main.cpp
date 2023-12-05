#include<bits/stdc++.h>

using namespace std;

vector<string> primary2;
vector<string> outputs2;
vector<string> internals2;
map<string,vector<string>> adj_list2;
map<string,vector<double>> areas;
map<string,vector<double>> gate_del2;
map<string,double> delays2;
vector<string> all_sig;
vector<string> gates;


double calc_del2(string signal){
	if(delays2[signal] != -1){
		return delays2[signal];
	}
	else{
		if(adj_list2[signal].size() == 4){
				string type = adj_list2[signal][0];
				string imp = adj_list2[signal][adj_list2[signal].size()-1];
				int t_i = stoi(imp);
				double time = gate_del2[adj_list2[signal][0]][t_i];
				double i1 = calc_del2(adj_list2[signal][1]);
				double i2 = calc_del2(adj_list2[signal][2]);
				return max(i1,i2)+time;
		}
		else{
			if(adj_list2[signal][0]=="DFF"){
				return 0.0;
			}		
			else{
				string type = adj_list2[signal][0];
				string imp = adj_list2[signal][adj_list2[signal].size()-1];
				int t_i = stoi(imp);
				double time = gate_del2[adj_list2[signal][0]][t_i];
				return calc_del2(adj_list2[signal][1]) + time;
			}
		}
		
	}
	
}

double calc_area() {
    double ret = 0.0;
    for(auto signal:all_sig){
    	string type = adj_list2[signal][0];
    	if(type !="DFF"){
		string imp = adj_list2[signal][adj_list2[signal].size()-1];
		int t_i = stoi(imp);
		ret = ret + areas[adj_list2[signal][0]][t_i];
		}
    }
    return ret;
}


string max_signal(string signal){
	if(adj_list2[signal][0] == "INV"){
		return adj_list2[signal][1];
	}
	else{
		if(delays2[adj_list2[signal][1]] >= delays2[adj_list2[signal][2]]){
				return adj_list2[signal][1];
		}
		else{
				return adj_list2[signal][2];
		}
	}
}



void minimise(string signal,double allowed){
	if(delays2[signal]==0){
		return;
	}
	string type = adj_list2[signal][0] ;
	if(type == "DFF"){
		return;
	}
	string imp = adj_list2[signal].back();
	int i = stoi(imp);
	if(i<2 && delays2[signal] > allowed){
		i++;
		delays2[signal] = -1;
		adj_list2[signal][adj_list2[signal].size()-1] = to_string(i);
		for(auto j:all_sig){
			delays2[j] = calc_del2(j);
		}
		minimise(signal,allowed);
	}
	else if(i == 2 && delays2[signal] >= allowed){
		double p = delays2[signal];
		delays2[signal] = -1;
		string max_sig = max_signal(signal);
		minimise(max_sig,allowed - gate_del2[adj_list2[signal][0]][i]);
		for(auto j:all_sig){
			delays2[j] = calc_del2(j);
		}
	}
	else{
		return;
	}
}


double calc_del(map<string,double>& gate_del,map<string,double>& delays,map<string,vector<string>>& adj_list,string& signal){
	if(delays[signal] != -1){
		return delays[signal];
	}
	else{
		if(adj_list[signal].size() == 3){
				double i1 = calc_del(gate_del,delays,adj_list,adj_list[signal][1]);
				double i2 = calc_del(gate_del,delays,adj_list,adj_list[signal][2]);
				return max(i1,i2)+gate_del[adj_list[signal][0]];
		}
		else{
			if(adj_list[signal][0]=="DFF"){
				return 0.0;
			}		
			else{
				return calc_del(gate_del,delays,adj_list,adj_list[signal][1]) + gate_del[adj_list[signal][0]];
			}
		}
		
	}
	
}


int main(int argc, char *argv[]){

	char part = argv[1][0];

	if(part=='A'){
		vector<string> primary;
		vector<string> outputs;
		vector<string> internals;
		ifstream infile(argv[3]);
		map<string,double> gate_del;
		map<string,double> delays;
		double longest = 0.0;
		string token;
		map<string,vector<string>> adj_list;
		while (infile >> token) {
		if (token == "//") {
			infile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			continue;
		}
		string gate;
		double del;
		double area;
		infile >> gate >> del >> area;
		if(gate_del.find(gate) != gate_del.end()) {
			gate_del[gate] = min(gate_del[gate], del);
		} 
		else{
			gate_del[gate] = del;
			}
		}
		infile.close();
		
		ifstream infile1(argv[2]);
		//graph creation
		string line;
		while (getline(infile1, line)) {
			if (line.empty() || line.find("//")==1) {
				continue;
			}
			istringstream iss(line);
			string type, token;
			iss >> type;

			if(type == "PRIMARY_INPUTS"){
				while(iss >> token){
					primary.push_back(token);
				}
			} 
			else if(type == "PRIMARY_OUTPUTS"){
				while(iss >> token){
					outputs.push_back(token);
				}
			} 
			else if(type == "INTERNAL_SIGNALS"){
				while (iss >> token){
					internals.push_back(token);
					}
			}
			else{
			// store gate type , then inputs .....eg: AND2 A B C , adj_list[C] = {"AND2","A","B"}
				if(type == "INV"){		
					iss >> token;
					string in = token;
					iss >> token;
					string out = token;
					adj_list[out].push_back(type); 
					adj_list[out].push_back(in); 	
				}
				else if(type == "AND2"){		
					iss >> token;
					string in = token;
					iss >> token;
					string mid = token;
					iss >> token;
					string out = token;
					adj_list[out].push_back(type); 
					adj_list[out].push_back(in); 
					adj_list[out].push_back(mid); 	
				}
				else if(type == "OR2"){		
					iss >> token;
					string in = token;
					iss >> token;
					string mid = token;
					iss >> token;
					string out = token;
					adj_list[out].push_back(type); 
					adj_list[out].push_back(in); 
					adj_list[out].push_back(mid); 	
				}
				else if(type == "NAND2"){		
					iss >> token;
					string in = token;
					iss >> token;
					string mid = token;
					iss >> token;
					string out = token;
					adj_list[out].push_back(type); 
					adj_list[out].push_back(in); 
					adj_list[out].push_back(mid); 		
				}
				else if(type == "NOR2"){		
					iss >> token;
					string in = token;
					iss >> token;
					string mid = token;
					iss >> token;
					string out = token;
					adj_list[out].push_back(type); 
					adj_list[out].push_back(in); 
					adj_list[out].push_back(mid); 	
				}
				if(type == "DFF"){		
					iss >> token;
					string in = token;
					iss >> token;
					string out = token;
					adj_list[out].push_back(type); 
					adj_list[out].push_back(in); 	
				}
						
			}
		}
		
		infile1.close();
	
		for(auto i:primary){
			delays[i] = 0;
		}
		for(auto i:internals){
			delays[i] = -1;
		}
		for(auto i:outputs){
			delays[i] = -1;
		}
		
		
		
		for(auto i:internals){
			delays[i] = calc_del(gate_del,delays,adj_list,i);
		}
		
		for(auto i:outputs){
			delays[i] = calc_del(gate_del,delays,adj_list,i);
		}
		
		
		
		for (auto in : internals) {
			longest = max(delays[in],longest);
		}
		
		for (auto out : outputs) {
			longest = max(delays[out],longest);
		}
		
		ofstream outputFile(argv[4]);
		outputFile << longest << endl; 
		outputFile.close();
		
	}else if(part=='B'){
	
		double max_allowed;
		
		ifstream infile0(argv[4]);

		double temp;
		infile0>>temp;
		max_allowed = temp;
		infile0.close();

		ifstream infile(argv[3]);
		double longest;
		string token;
		while (infile >> token) {
			if (token == "//") {
				infile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				continue;
			}
			string gate;
			double del;
			double area;
			infile >> gate >> del >> area;
			if(gates.empty() or gates[gates.size()-1] != gate){
				gates.push_back(gate);
			}
			gate_del2[gate].push_back(del);
			areas[gate].push_back(area);
		}	
		
		infile.close();	
		for (auto gate:gates){
			sort(gate_del2[gate].begin(),gate_del2[gate].end());
			sort(areas[gate].begin(),areas[gate].end());
			reverse(gate_del2[gate].begin(),gate_del2[gate].end());
		}
		ifstream infile1(argv[2]);
		
		//graph creation	
		string line;	
		while (getline(infile1, line)) {
			if (line.empty() || line.find("//")==1) {
				continue;
			}
			istringstream iss(line);
			string type, token;
			
			iss >> type;
			if(type == "PRIMARY_INPUTS"){
				while(iss >> token){
					primary2.push_back(token);
				}
				
			} 
			else if(type == "PRIMARY_OUTPUTS"){
				while(iss >> token){
					outputs2.push_back(token);
					
					all_sig.push_back(token);
				}
			} 
			else if(type == "INTERNAL_SIGNALS"){
				while (iss >> token){
					internals2.push_back(token);
					all_sig.push_back(token);
					}
			}
			else{
			// store gate type , then inputs .....eg: AND2 A B C , adj_list2[C] = {"AND2","A","B"}
				if(type == "INV"){		
					iss >> token;
					string in = token;
					iss >> token;
					string out = token;
					adj_list2[out].push_back(type); 
					adj_list2[out].push_back(in);
					adj_list2[out].push_back("0");   	
				}
				else if(type == "AND2"){		
					iss >> token;
					string in = token;
					iss >> token;
					string mid = token;
					iss >> token;
					string out = token;
					adj_list2[out].push_back(type); 
					adj_list2[out].push_back(in); 
					adj_list2[out].push_back(mid);
					adj_list2[out].push_back("0");   	
				}
				else if(type == "OR2"){		
					iss >> token;
					string in = token;
					iss >> token;
					string mid = token;
					iss >> token;
					string out = token;
					adj_list2[out].push_back(type); 
					adj_list2[out].push_back(in); 
					adj_list2[out].push_back(mid);
					adj_list2[out].push_back("0"); 	
				}
				else if(type == "NAND2"){		
					iss >> token;
					string in = token;
					iss >> token;
					string mid = token;
					iss >> token;
					string out = token;
					adj_list2[out].push_back(type); 
					adj_list2[out].push_back(in); 
					adj_list2[out].push_back(mid);
					adj_list2[out].push_back("0");  		
				}
				else if(type == "NOR2"){		
					iss >> token;
					string in = token;
					iss >> token;
					string mid = token;
					iss >> token;
					string out = token;
					adj_list2[out].push_back(type); 
					adj_list2[out].push_back(in); 
					adj_list2[out].push_back(mid);
					adj_list2[out].push_back("0");  	
				}
				if(type == "DFF"){		
					iss >> token;
					string in = token;
					iss >> token;
					string out = token;
					adj_list2[out].push_back(type); 
					adj_list2[out].push_back(in);
				}
						
			}
		}
		
		infile1.close();
		for(auto i:primary2){
			delays2[i] = 0;
		}
		for(auto i:internals2){
			delays2[i] = -1;
		}
		for(auto i:outputs2){
			delays2[i] = -1;
		}
		for(auto i:all_sig){
			delays2[i] = calc_del2(i);

		}
		for(auto i:all_sig){
			minimise(i,max_allowed);
		}
		double area = calc_area();

		ofstream outputFile(argv[5]);
		outputFile << area << endl; 
		outputFile.close();

	}

	
	return 0;
}
