//============================================================================
// Name        : CS300_ProjectTwo.cpp
// Author      : Kaylin Hooper
// Description : CS-300 Project Two
//============================================================================

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

// structure to hold course information
struct Course {
	string courseNumber;
	string courseName;
	vector<string> prereqList;
};

// Internal structure for tree node
struct Node {
	Course course;
	Node* left;
	Node* right;

	// default constructor
	Node() {
		left = nullptr;
		right = nullptr;
	}

	// initialize with a course
	Node(Course aCourse) :
		Node() {
		course = aCourse;
	}
};

// Binary Search Tree Implementation 

class BinarySearchTree {

private:
	Node* root;
	void addNode(Node* node, Course course);
	void inOrder(Node* node);

public:
	BinarySearchTree();
	virtual ~BinarySearchTree();
	void InOrder();
	void Insert(Course course);
	Course Search(string courseNumber);
};

// Default Constructor
BinarySearchTree::BinarySearchTree() {

	// initialize housekeeping variables
	root = nullptr;
}

// Destructor
BinarySearchTree::~BinarySearchTree() {

}

// Traverse the tree in order
void BinarySearchTree::InOrder() {

	// call inOrder function and pass root
	inOrder(root);
}

void BinarySearchTree::Insert(Course course) {

	// if root is empty
	if (root == nullptr) {
		// construct root from the course
		root = new Node(course);
	}
	else {
		// add Node, constructing with root and course
		this->addNode(root, course);
	}
}

void BinarySearchTree::addNode(Node* node, Course course) {
	// Starting at root node, if the bid value is less than the node's, go left
	if (node->course.courseNumber.compare(course.courseNumber) > 0) {
		// If no left node exists, create a left node
		if (node->left == nullptr) {
			// this node becomes left
			node->left = new Node(course);
		}
		// If a left node does exist, traverse down
		else {
			this->addNode(node->left, course);
		}
	}
	else {
		//If no right node exists, create a right node
		if (node->right == nullptr) {
			// this node becomes right
			node->right = new Node(course);
		}
		// If a right node does exist, traverse down
		else {
			this->addNode(node->right, course);
		}
	}
}

void BinarySearchTree::inOrder(Node* node) {
	// if node is not equal to nullptr, meaning it exists
	// In Order traversal = left side, visit root, right side
	// visit root = output course number and course name
	if (node != nullptr) {
		inOrder(node->left);
		cout << node->course.courseNumber << ", " << node->course.courseName << endl;
		inOrder(node->right);
	}
}

// Search for a Course
Course BinarySearchTree::Search(string courseNumber) {

	// Start search at the root
	Node* currentNode = root;

	// keep looping downwards until bottom reached or matching courseNumber found
	while (currentNode != nullptr) {
		// if match found, return current course
		if (currentNode->course.courseNumber == courseNumber) {
			return currentNode->course;
		}
		// if courseNumber is smaller than current node, traverse left
		if (courseNumber < currentNode->course.courseNumber) {
			currentNode = currentNode->left;
		}
		// else, it is larger, so traverse right
		else {
			currentNode = currentNode->right;
		}
	}
}

// Display Course information to the console
void displayCourse(Course course) {
	cout << course.courseNumber << ", " << course.courseName << endl;
	cout << "Prerequisites: ";

	// If the course has no prerequisites 
	for (const string& prereq : course.prereqList) {
		if (prereq.empty()) {
			cout << "None" << endl;
		}
		else {
			// Print prerequisites, starting at index 0
			cout << course.prereqList.at(0);

			// Then start at index 1
			for (unsigned int i = 1; i < course.prereqList.size(); ++i) {
				cout << ", " << course.prereqList.at(i);
				cout << endl;
			}
		}
	}
}


// Load a CSV file and parse each line, split into tokens
void loadCourses(string fileName, BinarySearchTree* bst) {

	// Define variables
	string line;
	string token;

	// open the file
	ifstream myfile;
	myfile.open(fileName, ios::in);

	// insert data into course structure
	if (myfile.is_open()) {
		// Display process to console
		cout << "Loading CSV file " << fileName << endl;

		// While loop to read every line
		while (getline(myfile, line)) {
			vector <string> tokens;
			stringstream ss(line);

			// Tokenize at comma 
			// Increments prereqCount at each comma
			// End of line, resets count
			while (getline(ss, token, ',')) {
				tokens.push_back(token);
			}

			// Structure Variables
			Course course = Course();
			course.courseNumber = tokens[0];
			course.courseName = tokens[1];

			// If course as prerequisites, add to the course
			if (tokens.size() > 2) {
				for (int i = 2; i < tokens.size(); i++) {
					course.prereqList.push_back(tokens[i]);
				}
			}
			// Add this course to the end
			bst->Insert(course);
		}
		cout << "Loaded CSV file " << fileName << " Successfully." << endl;
	}
	else {
		// Print error if file not opened
		cout << "Error. CSV file " << fileName << " not found." << endl;
	}

	// close the file
	myfile.close();

}

void printMenu() {
	cout << endl;
	cout << "Menu:" << endl;
	cout << "-----------------------------------" << endl;
	cout << "  1. Load Data Structure" << endl;
	cout << "  2. Print Course List" << endl;
	cout << "  3. Print Course" << endl;
	cout << "  9. Exit" << endl;
	cout << "-----------------------------------" << endl;
	cout << "Enter choice: ";
}

int main() {
	BinarySearchTree* bst;
	bst = new BinarySearchTree();
	Course course;
	string courseInput;

	// Define file variable
	string fileName;

	// Use given ABCU Advising program input
	fileName = "CourseProgram.csv";

	// bool to keep the loop
	bool programExit = false;

	cout << "Welcome to the course planner." << endl;
	do {

		int choice = 0;
		printMenu();
		cin >> choice;
		// validate user input as an integer
		if (!(cin >> choice)) {
			cin.clear();
			cin.ignore();
		}
		// validate that it is a menu option
		while ((choice < 0 || choice > 3) && choice != 9) {
			cout << choice << " is not a valid option." << endl;
			cout << endl;
			printMenu();
			cin >> choice;
		}
		switch (choice) {
		case 1:
			loadCourses(fileName, bst);
			break;

		case 2:
			cout << "Here is a sample schedule: " << endl;
			bst->InOrder();
			break;

		case 3:
			cout << "What course do you want to know about? " << endl;
			cin >> courseInput;

			// Convert user input to uppercase for proper search.
			// Referenced DigitalOcean's published tutorial: https://www.digitalocean.com/community/tutorials/string-uppercase-lowercase-c-plus-plus
			transform(courseInput.begin(), courseInput.end(), courseInput.begin(), ::toupper);
			cout << endl;

			//Call search method with entered course
			course = bst->Search(courseInput);

			if (!course.courseNumber.empty()) {
				displayCourse(course);
				break;
			}
			else {
				cout << "Course " << courseInput << " not found." << endl;
				break;
			}
		case 9:
			cout << "Thank you for using the course planner!" << endl;
			programExit = true;
		}
	} while (!programExit);
	return 0;
}