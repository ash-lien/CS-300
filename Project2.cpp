#include <iostream>
#include <time.h>

#include "CSVparser.hpp"

using namespace std;

//structure to hold course information
struct Course {
	string courseNum;
	string courseName;
	vector<string> preReqs;
	Course() {}
};

//Internal structure for tree node
struct Node {
	Course course;
	Node* left;
	Node* right;
	
	//default constructor
	Node() {
		left = nullptr;
		right = nullptr;
	}
	//initialize with a course
	Node(Course aCourse) :
		Node() {
		this->course = aCourse;
	}
};

class BinarySearchTree {
private :
	Node* root;

	void addNode(Node* node, Course course);
	void printSchedule(Node* node);
	void courseInfo(Node* node, string courseNum);

public:
	BinarySearchTree();
	virtual ~BinarySearchTree();
	void DeleteRecursive(Node* node);
	void Insert(Course course);
	int NumPrereqCourse(Course course);
	void printSchedule();
	void courseInfo(string courseNum);
};

//default constructor
BinarySearchTree::BinarySearchTree() {
	//initialize housekeeping variable
	root = nullptr;
}
//destructor
BinarySearchTree::~BinarySearchTree() {
	DeleteRecursive(root);
}
void BinarySearchTree::DeleteRecursive(Node* node) {
	if (node) {
		DeleteRecursive(node->left);
		DeleteRecursive(node->right);
		delete node;
	}
}
void BinarySearchTree::Insert(Course course) {
	if (root == nullptr) {
		root = new Node(course);
	}
	else {
		this->addNode(root, course);
	}
}
int BinarySearchTree::preReqs(Course course) {
	int count = 0;
	for (unsigned int i = 0; i < course.preReqs.size(); i++) {
		if (course.preReqs.at(i).length() > 0) {
			count++;
		}
	}
	return count;
}
void BinarySearchTree::printSchedule() {
	this->printSchedule(root);
}
void BinarySearchTree::courseInfo(string courseNum) {
	this->courseInfo(root, courseNum);
}
void BinarySearchTree::addNode(Node* node, Course course) {
	if (node->course.courseNum.compare(course.courseNum) > 0) {
		if (node->left == nullptr) {
			node->left = new Node(course);
		}
		else {
			this->addNode(node->left, course);
		}
	}
	else {
		if (node->right == nullptr) {
			node->right = new Node(course);
		}
		else {
			this->addNode(node->right, course);
		}
	}
}
void BinarySearchTree::printSchedule(Node* node) {
	if (node != nullptr) {
		printSchedule(node->left);
		cout << node->course.courseNum << ", " << node->course.courseName << endl;
		printSchedule(node->right);
	}
	return;
}
void BinarySearchTree::courseInfo(Node* curr, string courseNum) {
	while (curr != nullptr) {
		if (curr->course.courseNum.compare(courseNum) == 0) {
			cout << endl << curr->course.courseNum << ", " << curr->course.courseName << endl;
			unsigned int size = preReqs(curr->course);
			cout << "Prerequisites: ";
			for (i = 0; i < size; i++) {
				cout << curr->course.preReqs.at(i);
				if (i != size - 1) {
					cout << ", ";
				}
				if (i == 0) {
					cout << "No Prerequisites required." << endl;
					return;
				}
			}
		}
		else if (courseNum.compare(curr->course.courseNum) < 0) {
			curr = curr->left;
		}
		else {
			curr = curr->right;
		}
	}
	cout << "Course " << courseNum << " not found." << endl;
}
void loadCourses(string csvPath, BinarySearchTree* bst) {
	cout << "Loading CSV file " << csvPath << endl;

	// initialize the CSV Parser using the given path
	csv::Parser file = csv::Parser(csvPath);

	// read and display header row - optional
	vector<string> header = file.getHeader();
	for (auto const& c : header) {
		cout << c << " | ";
	}
	cout << "" << endl;

	try {
		// loop to read rows of a CSV file
		for (unsigned int i = 0; i < file.rowCount(); i++) {

			// Create a data structure and add to the collection of bids
			Course course;
			course.courseNum = file[i][1];
			course.courseName = file[i][0];
			// push this course to the end
			bst->Insert(course);
		}
	}
	catch (csv::Error& e) {
		std::cerr << e.what() << std::endl;
	}
}
int main(int argc, char* argv[]) {

	// process command line arguments
	string csvPath, courseId;
	switch (argc) {
	case 2:
		csvPath = argv[1];
		break;
	case 3:
		csvPath = argv[1];
		courseId = argv[2];
		break;
	default:
		csvPath = "";
		break;
	}
	BinarySearchTree* bst;
	bst = new BinarySearchTree();
	Course course;

	int choice = 0;
	while (choice != 9) {
		cout << "Welcome to the course planner!" << endl;
		cout << "  1. Load Data Structure" << endl;
		cout << "  2. Print Course List" << endl;
		cout << "  3. Print Course" << endl;
		cout << "  9. Exit" << endl;
		cout << endl;
		cout << "What would you like to do?";
		cin >> choice;

		switch (choice) {

		case 1:
			// Complete the method call to load the courses
			loadCourses(csvPath, bst);
			break;

		case 2:
			bst->printSchedule();
			break;

		case 3:
			course = bst->Search(courseId);


			if (!course.courseId.empty()) {
				displayCourse(course);
			}
			else {
				cout << "Course " << courseId << " not found." << endl;
			}

			break;
		}
	}

	cout << "Good bye." << endl;

	return 0;
}

