#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>


/*
 * Utils
 */
template <typename T>
class LinkedList {

public:
    struct Node {
        T data;
        Node* next;
        Node(T d): data(d), next(nullptr) {}
    };

    int size;
    Node* head;

    LinkedList() : head(nullptr), size(0) {}

    Node* getHead() {
        return head;
    }

    void addFront(T data) {
        Node* newNode = new Node(data);
        newNode->next = this->head;
        this->head = newNode;
        this->size++;
    }

    void addTail(T data) {
        Node* newNode = new Node(data);

        if (this->head == nullptr) {
            this->head = newNode;
        } else {
            Node* cur = this->head;
            while (cur->next != nullptr) {
                cur = cur->next;
            }
            
            cur->next = newNode;
        }
        this->size++;
    }

    T peekFront() {
        if (this->isEmpty()) {
            std::cerr << "The list is empty!" << std::endl;
            exit(1);
        }
        return this->head->data;
    }

    T peekTail() {
        if (this->isEmpty()) {
            std::cerr << "The list is empty!" << std::endl;
            exit(1);
        }

        Node* cur = this->head;
        while (cur->next != nullptr) {
            cur = cur->next;
        }
        
        return cur->data;
    }

    T removeFront() {
        if (this->head == nullptr) {
            std::cerr << "List is empty"  << std::endl;
            exit(1);
        }

        Node remove = this->head;
        T data = remove.data;
        this->head = remove->next;
        delete remove;
        size--;

        return data;
    }

    T removeTail() {
        if (this->isEmpty()) {
            std::cerr << "The list is empty." << std::endl;
            exit(1);
        }

        Node* cur = this->head;
        while (cur->next != nullptr) {
            cur = cur->next;
        }

        T data = cur->data;
        delete cur;
        size--;
        return data;
    }

    bool remove_nth(int n) {
        if (n >= this->getSize()) {
            return false;
        }
        
        Node* prev = nullptr;
        Node* cur = this->head;

        for(int i = 0; i < n - 1; i++) {
            prev = cur;
            cur = cur->next;
        }

        Node* temp = cur;
        T data = temp->data;

        if (prev) {
            prev->next = cur->next;
        } else {
            head = cur->next;
        }

        delete temp;
        size--;

        return true;
    }

    T get_nth_node(int n) {
        if (n > this->getSize())  {
            std::cerr << "Out of range." << std::endl;
        } 

        Node* cur = this->head;
        int count = 1;
        while (count != n) {
            if (count == n) {
                break;
            }
            cur = cur->next;
            count++;
        }

        return cur->data;
    }

    std::vector<T> find_all(std::string keyword) {
        std::vector<T> ret;

        Node* cur = this->head;
        while (cur != nullptr) {
            if (cur->data.get_content().find(keyword) != std::string::npos) {
                ret.push_back(cur->data);
            }
            cur = cur->next;
        }
        
        return ret;
    }

    
    int getSize() {
        return this->size;
    }

    bool isEmpty() {
        return this->size == 0;
    }

    void printList() {
        Node* curr = this->head;

        while (curr != nullptr) {
            std::cout << curr->data;
            curr = curr->next;
        }
        
    }

    ~LinkedList() {
        while (head != nullptr) {
            Node* tmp = head;
            head = head->next;
            delete tmp;
        }
    }
};

class Version {
private:
    int aVersion_number;
    size_t aHashcode;
    std::string aContent;

public: 
    static int count;

    // update count here
    Version(std::string pContent): aVersion_number(this->count), aContent(pContent) {
        this->count++;
        std::hash<std::string> hash_string;
        std::hash<int> hash_int;
        aHashcode = hash_string(pContent) ^ hash_int(this->aVersion_number);
    }

    Version(int pVersion_number, size_t pHashcode, std::string pContent) {
        aVersion_number = pVersion_number;
        aHashcode = pHashcode;
        aContent = pContent;
    }


    int get_version_number() const {
        return aVersion_number;
    }

    size_t get_hashcode() const {
        return aHashcode;
    }

    std::string get_content() const {
        return aContent;
    }

    friend std::ostream& operator<<(std::ostream& os, const Version& v) {
        os << "Version number: " << v.aVersion_number << "\nHash value: " << v.aHashcode << "\nContent: " << v.aContent << std::endl;
        return os;
    }
};

int Version::count = 1;


class VersionList: public LinkedList<Version> {
public:
    VersionList() {
        LinkedList<Version>();
    }

    bool remove_version(int version_number) {
        Node* prev = nullptr;
        Node* cur = LinkedList<Version>::getHead();

        
        while (cur != nullptr) {
            if (cur->data.get_version_number() == version_number) {
                // remove the node
                Node* temp = cur;
                if (prev) {
                    prev->next = cur->next;
                } else {
                    head = cur->next;
                }
                delete temp;
                this->size--;

                return true;
            }
            prev = cur;
            cur = cur->next;
        }

        return false;
    }
};

class Git322 {
protected:
    std::string aFilename;
    VersionList mylist;

    bool load_helper(int version) {
        if (version <= 0 || version > this->mylist.getSize()) {
            return false;
        }
        Version node = this->mylist.get_nth_node(version);
        std::ofstream file(this->aFilename, std::ios::trunc);

        if(!file) {
            std::cerr << "Failed to open file." << std::endl;
            return false;
        }

        file << node.get_content() << std::endl;

        file.close();
        return true;
    }

    bool compare_helper(int version1, int version2) {
        if (version1 <= 0 || version1 > this->mylist.getSize() || version2 <= 0 || version2 > this->mylist.getSize()) {
            return false;
        }

        Version version_node1 = this->mylist.get_nth_node(version1);
        Version version_node2 = this->mylist.get_nth_node(version2);

        std::string content1 = version_node1.get_content();
        std::string content2 = version_node2.get_content();

        std::vector<std::string> lines1, lines2;
        std::string line;
        std::istringstream iss1(content1), iss2(content2);

        while (std::getline(iss1, line)) {
            lines1.push_back(line);
        }

        while (std::getline(iss2, line)) {
            lines2.push_back(line);
        }

        int i = 0;

        while(i < lines1.size() || i < lines2.size()) {
            if (i >= lines1.size()) {
               std::cout << "Line " << (i+1) << ": <Empty line> <<>> " << lines2[i] << std::endl;
            } else if (i >= lines2.size()) {
               std::cout << "Line " << (i+1) << ": " << lines1[i] << " <<>> <Empty line>" << std::endl;
            } else if (lines1[i] != lines2[i]) {
                std::cout << "Line " << (i+1) << ": " << lines1[i] << " <<>> " << lines2[i] << std::endl;
            } else {
                std::cout << "Line " << (i+1) << ": " << "<Identical>" << std::endl;
            }

            i++;
        }

        return true;
    }

public:
    Git322(std::string pFilename): aFilename(pFilename) {
        mylist = VersionList();
    }

    // Add to versions
    void add(std::string content) {
        if (!this->mylist.isEmpty()) {
            if (content.compare("") == 0 || this->mylist.peekTail().get_content().compare(content) == 0) {
            std::cout << "git322 did not detect any change to your file and will not create a new version." << std::endl;
            return;
            }
        }

        Version* v_node = new Version(content);
        this->mylist.addTail(*v_node);
        std::cout << "Your content has been added successfully." << std::endl;
        return;
    }


    void print() {
        std::cout << "Number of versions: " << this->mylist.getSize() << std::endl;
        this->mylist.printList();
    }

    // this method should search for the vorrect version number and del it instead
    void remove(int version) {
        bool return_status = this->mylist.remove_version(version);
        if (!return_status) {
            std::cout << "Please enter a valid version number." << std::endl;
        } else {
            std::cout << "Version " << version << " deleted successfully." << std::endl;
        }

    }

    void load(int version) {
        bool return_status = load_helper(version);
        if (return_status) {
            std::cout << "Version " << version << " loaded successfully. Please refresh your text editor to see the changes." << std::endl;
        } else {
            std::cout << "Please enter a valid version number. If you are not sure please press 'p' to lst all valid version number." << std::endl;
        }
    }

    void search(std::string keyword) {
        std::vector<Version> results = this->mylist.find_all(keyword);

        if (results.size() <= 0) {
            std::cout << "Your keyowrd '" << keyword << "' was not found in any version." << std::endl;
        } else {
            std::cout << "The keyowrd" << keyword << "' has been found in the following version: " << std::endl;

            for (int i = 0 ; i < results.size(); i++) {
                std::cout << results[i] << std::endl;
            }
        }

    }

    void compare(int version1, int version2) {
        bool return_status = compare_helper(version1, version2);
        if (!return_status) {
            std::cout << "Please enter a valid version number. If you are not sure please press 'p' to list all valid version number." << std::endl;
        }
    }

};

class EnhancedGit322 : public Git322 {
private:
    std::string aSave_dest;

    void load_saved() {
        std::ifstream file(aSave_dest);

        std::string line;
        // if(file.eof()) {return; }
        getline(file, line);
        if (line == "") {return;}
        int version_count = std::stoi(line);

        int node_count = 0;
        
        while(true) {
            char c = file.get();
            if (file.eof()) break;
            int reading_state = 0;

            std::vector<std::string> values;
            std::string token = "";

            while(reading_state != 3) {
                if (c == '|') {
                    values.push_back(token);
                    token.clear();
                    reading_state++;
                } else {
                    token += c;
                }
                c = file.get();
            }

            values.push_back(token);

            // std::cout << "DEBUG: |" << values[0] << "|" << std::endl;
            int version_number = std::stoi(values[0]);
            size_t hash_code = std::stoul(values[1]);
            std::string content = values[2];
            
            Version* v_node = new Version(version_number, hash_code, content);
            this->mylist.addTail(*v_node);
            node_count++;
        }

        Version::count = version_count;

        file.close();
    }

    void save() {
        std::ofstream file(this->aSave_dest, std::ios::out);
        
        file << Version::count << "\n";

        VersionList::Node* cur = this->mylist.getHead();

        while (cur != nullptr) {
            file << cur->data.get_version_number() << "|" << cur->data.get_hashcode() << "|" << cur->data.get_content() << "|\n";
            cur = cur->next;
        }
        
        file.close();
        return;    
    }

public: 
    EnhancedGit322(std::string pFilename, std::string pSave_dest) : Git322(pFilename), aSave_dest(pSave_dest) {
        // Try to retrieve from saved
        load_saved();
    }

    void add(std::string content) {
        Git322::add(content);
        save();
    }

    void remove(int version) {
        Git322::remove(version);
        save();
    }

};

std::string load_file_content(std::string filename) {
    std::ifstream file(filename);

    if(!file.is_open()) {
        std::cerr << "Failed to open file" << std::endl;
        exit(1);
    }

    std::string file_contents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    return file_contents;
}

void displayWelcomeMsg() {
    std::cout << "Welcome to the Comp322 file versioning system!" << std::endl;
    std::cout << std::endl;
    std::cout << "To add the content of your file to version control press 'a'" << std::endl;
    std::cout << "To remove a version press 'r'" << std::endl;
    std::cout << "To load a version press 'l'" << std::endl;
    std::cout << "To print to the screen the detailed list of all versions press 'p'" << std::endl;
    std::cout << "To compare any 2 versions press 'c'" << std::endl;
    std::cout << "To search versions for a keyword press 'a'" << std::endl;
    std::cout << "To exit press 'e'" << std::endl;
}


void run() {
    std::string filename = "file.txt";

    displayWelcomeMsg();
    Git322 git322(filename);

    std::string file_content;

    char user_select_function;
    int user_function_input1;
    int user_function_input2;
    std::string user_keyword_input;

    while (true) {
        std::cin >> user_select_function;

        switch (user_select_function) {
            case 'a':
                file_content = load_file_content(filename);
                git322.add(file_content);
                break;

            case 'p':
                git322.print();
                break;

            case 'l':
                std::cout << "Which version would you like to load?" << std::endl;
                std::cin >> user_function_input1;
                git322.load(user_function_input1);
                break;
        

            case 'c':
                std::cout << "Please enter the number of the first version to compare: "  << std::endl;
                std::cin >> user_function_input1;
                std::cout << "Please enter the number of the second version to compare: " << std::endl;
                std::cin >> user_function_input2;

                git322.compare(user_function_input1, user_function_input2);
                break;

            case 's':
                std::cout << "Please enter the keyword that you are looking for: " << std::endl;
                std::cin >> user_keyword_input;
                git322.search(user_keyword_input);
                break;

            case 'r':
                std::cout << "Enter the number of the version that you want to delete:" << std::endl;
                std::cin >> user_function_input1;
                git322.remove(user_function_input1);
                break;

            case 'e':
                std::cout << "Goodbye!" << std::endl;
                return;
                
            default:
                std::cout << "Unknown option " << user_select_function << std::endl;
                break;
        }
    }

}



int main() {
    std::string filename = "file.txt";
    std::string save_dest = "save.txt";

    displayWelcomeMsg();
    EnhancedGit322 git322(filename, save_dest);

    std::string file_content;

    char user_select_function;
    int user_function_input1;
    int user_function_input2;
    std::string user_keyword_input;

    while (true) {
        std::cin >> user_select_function;

        switch (user_select_function) {
            case 'a':
                file_content = load_file_content(filename);
                git322.add(file_content);
                break;

            case 'p':
                git322.print();
                break;

            case 'l':
                std::cout << "Which version would you like to load?" << std::endl;
                std::cin >> user_function_input1;
                git322.load(user_function_input1);
                break;
        

            case 'c':
                std::cout << "Please enter the number of the first version to compare: "  << std::endl;
                std::cin >> user_function_input1;
                std::cout << "Please enter the number of the second version to compare: " << std::endl;
                std::cin >> user_function_input2;

                git322.compare(user_function_input1, user_function_input2);
                break;

            case 's':
                std::cout << "Please enter the keyword that you are looking for: " << std::endl;
                std::cin >> user_keyword_input;
                git322.search(user_keyword_input);
                break;

            case 'r':
                std::cout << "Enter the number of the version that you want to delete:" << std::endl;
                std::cin >> user_function_input1;
                git322.remove(user_function_input1);
                break;

            case 'e':
                std::cout << "Goodbye!" << std::endl;
                return 0;
                
            default:
                std::cout << "Unknown option " << user_select_function << std::endl;
                break;
        }
    }

}
