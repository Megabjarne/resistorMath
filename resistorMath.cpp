#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <cstdint>
#include <iomanip>

using namespace std;

enum nodeTypes{resistor, parallell, series};
int modifierCount = 3;
char modifiers[] = { 'K','M','G' };
double modifierValues[] = { 1000.0,1000000.0,1000000000.0 };


class calculationNode{
	
	public:
	nodeTypes type;
	double value;
	calculationNode* child1;
	calculationNode* child2;
	double getValueNode1();
	double getValueNode2();
	double getValue();
	calculationNode(nodeTypes _type, calculationNode* _child1, calculationNode* _child2);
	calculationNode(double _value);
};

string removeBrackets(string s);
void splitString(string s, vector<string>* targetLocation);
bool isNumber(string s);
double parseDouble(string s);
calculationNode* parse(string s);
string modify(double value);
double deModify(string s);
double getModifier(char c);
bool isModifier(char c);

int main(int nargs, char** args){
	string input="";
	for (int i=1;i<nargs;i++){
		input += string(args[i]);
	}
	cout<<"Parsing"<<endl;
	calculationNode* temp = parse(input);
	cout<<"Calculating";
	double ans = temp->getValue();
	cout<<endl;
	cout<<"Answer: "<<modify(ans)<<" Ohms ("<<ans<<" ohms)"<<endl;
	return 0;
}

string modify(double value) {
	for (int i = modifierCount - 1; i >= 0; i--) {
		if ((value / modifierValues[i]) >= 1) {
			stringstream ss;
			ss << fixed << setprecision(2) << (value/modifierValues[i]);
			return ss.str() + modifiers[i];
		}
	}
	return to_string(value);
}

double deModify(string s) {
	if (isModifier(s[s.size() - 1])) {
		return parseDouble(s.substr(0, s.size() - 1))*getModifier(s[s.size()-1]);
	}
	return parseDouble(s);
}

bool isModifier(char c) {
	for (int i = 0; i < modifierCount; i++) {
		if (modifiers[i] == c || modifiers[i] - 'A' + 'a' == c) {
			return true;
		}
	}
	return false;
}

double getModifier(char c) {
	for (int i = 0; i < modifierCount; i++) {
		if (modifiers[i] == c || modifiers[i] - 'A' + 'a' == c) {
			return modifierValues[i];
		}
	}
	return 0;
}

calculationNode* parse(string s){
	vector<string> parts;
	splitString(s, &parts);
	if (parts.size()==1){
		return new calculationNode(deModify(parts[0]));
	}
	else if (parts.size()==3)
	{
		nodeTypes operationType = (parts[1]=="|") ? parallell : series;
		return new calculationNode(operationType, parse(parts[0]), parse(parts[2]));
	}
	cout << "Got unexpected split-result: " << parts.size() << endl;
	exit(-1);
}

double calculationNode::getValue(){
	cout<<".";
	if (this->type == resistor){
		return value;
	}
	if (this->type == parallell){
		return 1/(1/(this->getValueNode1()) + 1/(this->getValueNode2()));
	}
	if (this->type == series){
		return this->getValueNode1() + this->getValueNode2();
	}
	cout<<endl<<"getValue failed, type is unknown"<<endl;
	exit (EXIT_FAILURE);
}

double calculationNode::getValueNode1(){
	if (child1!=NULL){
		return child1->getValue();
	}else{
		cout<<"Child 1 is a nullpointer, exiting"<<endl;
		exit (EXIT_FAILURE);
	}
}

double calculationNode::getValueNode2(){
	if (child2!=NULL){
		return child2->getValue();
	}else{
		cout<<"Child 2 is a nullpointer, exiting"<<endl;
		exit (EXIT_FAILURE);
	}
}

string removeBrackets(string s){
	int bracketLevel=0;
	for (int i=0;i<s.size();i++){
		if (s[i]=='('){
			bracketLevel++;
		}
		if (s[i]==')'){
			bracketLevel--;
		}
		if (bracketLevel==0 && i!=s.size()-1){
			return s;
		}
	}
	if (bracketLevel != 0) {
		cout << "Bracketing error, for shame man, for shame" << endl;
		exit(-1);
	}
	else {
		if (s[0] == '(' && s[s.size()-1] == ')') {
			return s.substr(1, s.size() - 2);
		}
		else {
			return s;
		}
	}
}

void splitString(string s, vector<string>* targetLocation){
	s = removeBrackets(s);
	if (isNumber(s)){
		targetLocation->push_back(s);
		return;
	}
	int bracketHeight = 0;
	for (int i=0;i<s.size();i++){
		if (s[i]=='('){bracketHeight++;}
		if (s[i]==')'){bracketHeight--;}
		if (bracketHeight==0 && (s[i]=='|' || s[i]=='&')){	//not inside bracket, and reached an operator
			targetLocation->push_back(removeBrackets(s.substr(0,i)));
			targetLocation->push_back(s.substr(i,1));
			targetLocation->push_back(removeBrackets(s.substr(i+1,s.size()-i-1)));
			return;
		}
	}
	cout << "Could not split string: '" << s << "'" << endl;
	exit(-1);
}

bool isNumber(string s){
	for (int i=0;i<s.size()-1;i++){
		if (!(s[i]>='0' && s[i]<='9')){
			return false;
		}
	}
	if ((s[s.size() - 1] >= '0' && s[s.size() - 1] <= '9') || isModifier(s[s.size()-1])) {
		return true;
	}
	return false;
}

double parseDouble(string s){
	double retVal=0;
	for (int i=0;i<s.size();i++){
		retVal*=10;	//shift one digit left
		retVal+=s[i]-'0';//convert char to digit and add it
	}
	return retVal;
}

calculationNode::calculationNode(nodeTypes _type, calculationNode* _child1, calculationNode* _child2){
	this->type=_type;
	this->child1=_child1;
	this->child2=_child2;
	this->value=0;
}

calculationNode::calculationNode(double _value){
	this->type=resistor;
	this->value=_value;
}









