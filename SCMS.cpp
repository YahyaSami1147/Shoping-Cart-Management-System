#include <iostream>
#include <string>
using namespace std;

// Structure to represent a product
struct Product {
    int id;            // Product ID
    string name;       // Product name
    double price;      // Product price
    int quantity;      // Product quantity in stock
};

// Node for the doubly linked list representing the shopping cart
class CartNode {
public:
    int productId;         // Product ID
    string productName;    // Product name
    double price;          // Product price
    int quantity;          // Product quantity in the cart
    CartNode* next;        // Pointer to the next node
    CartNode* prev;        // Pointer to the previous node

    // Constructor to initialize a cart node with product info
    CartNode(int id, string name, double p, int qty) : productId(id), productName(name), price(p), quantity(qty), next(nullptr), prev(nullptr) {}
};

// Stack class for storing actions to support undo functionality
class Stack {
public:
    struct Action {
        string actionType; // "add" or "remove"
        int productId;
        string productName;
        int quantity;
    };
private:
    Action actions[100];   // Array to hold actions
    int top;                // Index for the top of the stack

public:
    Stack() : top(-1) {}    // Initialize the stack with an empty state

    // Push an action to the stack
    void push(const string& actionType, int productId, const string& productName, int quantity) {
        if (top < 99) { 
            actions[++top] = {actionType, productId, productName, quantity};
        } else {
            cout << "Stack is full!\n";  // Error if the stack is full
        }
    }

    // Pop the top action from the stack
    Action pop() {
        if (top == -1) {
            return {"", -1, "", -1};  // No actions to undo
        } else {
            return actions[top--];  // Return the top action and remove it
        }
    }

    // Check if the stack is empty
    bool isEmpty() {
        return top == -1;
    }
};

// Queue class for managing the order of items placed
class Queue {
private:
    int* orderIds;          // Array to hold order IDs
    int front;              // Front of the queue
    int back;               // Back of the queue
    int capacity;           // Maximum capacity of the queue

public:
    Queue() : front(-1), back(-1), capacity(100) {  
        orderIds = new int[capacity];  // Dynamically allocate memory for the order IDs
    }

    // Enqueue an order ID to the queue
    void enqueue(int orderId) {
        if (back == capacity - 1) {
            cout << "Queue Overflow!\n";  // Error if the queue is full
            return;
        }
        back++;               // Move to the next position
        orderIds[back] = orderId;

        if (front == -1) {    // If it's the first element, set front to 0
            front++;
        }
    }

    // Dequeue an order ID from the queue
    void dequeue() {
        if (front == -1 || front > back) {
            cout << "Queue is empty!\n";  // Error if the queue is empty
            return;
        }
        front++;               // Move front to the next element
    }

    // Peek at the front element of the queue
    int peek() {
        if (front == -1 || front > back) {
            return -1;         // Return -1 if the queue is empty
        }
        return orderIds[front];  // Return the front element of the queue
    }

    // Check if the queue is empty
    bool isEmpty() {
        return front == -1 || front > back;
    }
};

// Structure for nodes in an AVL tree representing products
struct Node {
    Product product;        // Product data
    Node *left;             // Left child node
    Node *right;            // Right child node
    int height;             // Height of the node (for balancing)

    // Constructor to initialize an AVL tree node
    Node(Product p) : product(p), left(nullptr), right(nullptr), height(1) {}
};

// Helper functions for AVL tree balancing
int max(int a, int b) {
    return (a > b) ? a : b;
}

int height(Node *n) {
    return (n == nullptr) ? 0 : n->height;
}

int getBalance(Node *n) {
    if (n == nullptr)
        return 0;
    return height(n->left) - height(n->right);  // Balance factor
}

// Rotations for AVL tree balancing
Node *rightRotate(Node *y) {
    Node *x = y->left;
    Node *T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

Node *leftRotate(Node *x) {
    Node *y = x->right;
    Node *T2 = y->left;

    y->left = x;
    x->right = T2;

    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return y;
}

// Function to find the minimum value node in a tree (used for deletion)
Node* minValueNode(Node* node) {
    while (node->left != nullptr)
        node = node->left;
    return node;
}

// Insert a product into the AVL tree
Node* insert(Node* node, Product p) {
    if (node == nullptr) {
        return new Node(p);
    }

    if (p.id < node->product.id) {
        node->left = insert(node->left, p);
    } else if (p.id > node->product.id) {
        node->right = insert(node->right, p);
    } else {
        return node;  // Duplicates are not allowed
    }

    node->height = 1 + max(height(node->left), height(node->right));  // Update height

    int balance = getBalance(node);  // Check balance factor

    // Perform rotations if the tree becomes unbalanced
    if (balance > 1 && p.id < node->left->product.id)
        return rightRotate(node);

    if (balance < -1 && p.id > node->right->product.id)
        return leftRotate(node);

    if (balance > 1 && p.id > node->left->product.id) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    if (balance < -1 && p.id < node->right->product.id) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

// Search for a product by ID in the AVL tree
Node* search(Node* root, int id) {
    if (root == nullptr || root->product.id == id)
        return root;

    if (id < root->product.id)
        return search(root->left, id);

    return search(root->right, id);
}

// Delete a product by ID from the AVL tree
Node* deleteNode(Node* root, int id) {
    if (root == nullptr) return root;

    if (id < root->product.id)
        root->left = deleteNode(root->left, id);
    else if (id > root->product.id)
        root->right = deleteNode(root->right, id);
    else {
        if (root->left == nullptr || root->right == nullptr) {
            Node* temp = root->left ? root->left : root->right;
            delete root;
            return temp;
        } else {
            Node* temp = minValueNode(root->right);
            root->product = temp->product;
            root->right = deleteNode(root->right, temp->product.id);
        }
    }

    root->height = max(height(root->left), height(root->right)) + 1;

    int balance = getBalance(root);

    // Perform rotations to restore balance
    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);

    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);

    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

// Display all the products in the AVL tree (in-order traversal)
void displayProducts(Node* root) {
    if (root == nullptr) {
        return;
    }

    displayProducts(root->left);  // Visit left subtree

    cout << "Product ID: " << root->product.id
         << " | Name: " << root->product.name
         << " | Price: $" << root->product.price
         << " | Quantity: " << root->product.quantity << endl;

    displayProducts(root->right);  // Visit right subtree
}

// Class representing the shopping cart
class ShoppingCart {
private:
    CartNode* head;           // Head of the doubly linked list (cart items)
    Stack undoStack;          // Undo stack for cart actions

public:
    ShoppingCart() : head(nullptr) {}

    // Add an item to the shopping cart
    void addItem(int id, const string& name, double price, int qty) {
        CartNode* newNode = new CartNode(id, name, price, qty);
        if (!head) {
            head = newNode;
        } else {
            CartNode* temp = head;
            while (temp->next) {
                temp = temp->next;
            }
            temp->next = newNode;
            newNode->prev = temp;
        }
        undoStack.push("add", id, name, qty);  // Push to undo stack
        cout << name << " added to cart.\n";
    }

    // Modify the quantity of an item in the cart
    void modifyItem(int id, int newQty) {
        CartNode* temp = head;
        while (temp && temp->productId != id) {
            temp = temp->next;
        }

        if (temp) {
            temp->quantity = newQty;
            cout << "Quantity of " << temp->productName << " updated to " << newQty << ".\n";
        } else {
            cout << "Item not found in cart!\n";
        }
    }

    // Remove an item from the cart
    void removeItem(int id) {
        if (!head) {
            cout << "Cart is empty!\n";
            return;
        }

        CartNode* temp = head;
        while (temp && temp->productId != id) {
            temp = temp->next;
        }

        if (!temp) {
            cout << "Item not found in cart!\n";
            return;
        }

        undoStack.push("remove", temp->productId, temp->productName, temp->quantity);  // Push to undo stack

        if (temp->prev) {
            temp->prev->next = temp->next;
        } else {
            head = temp->next;
        }
        if (temp->next) {
            temp->next->prev = temp->prev;
 }

        delete temp;
        cout << "Item removed from cart.\n";
    }

    // Display the items in the cart
    void displayCart() {
        if (!head) {
            cout << "Cart is empty!\n";
            return;
        }
        CartNode* temp = head;
        cout << "\nShopping Cart:\n";
        while (temp) {
            cout << "ID: " << temp->productId << " | Name: " << temp->productName
                 << " | Price: $" << temp->price << " | Quantity: " << temp->quantity << endl;
            temp = temp->next;
        }
    }

    // Undo the last action
    void undo() {
        if (undoStack.isEmpty()) {
            cout << "Nothing to undo!\n";
            return;
        }

        Stack::Action lastAction = undoStack.pop();
        if (lastAction.actionType == "add") {
            // Restore the item that was added
            removeItem(lastAction.productId); // Remove the item from the cart
            cout << lastAction.productName << " removed from cart.\n";
        } else if (lastAction.actionType == "remove") {
            // Restore the item that was removed
            addItem(lastAction.productId, lastAction.productName, 0, lastAction.quantity); // Price is not needed here
            cout << lastAction.productName << " added back to cart.\n";
        }
    }

    CartNode* getHead() {
        return head;
    }
};

int main() {
    // Initialize product list using AVL tree
    Node* root = nullptr;

    // Adding products to the AVL tree
    Product p1 = {1, "Laptop", 999.99, 10};
    Product p2 = {2, "Smartphone", 699.99, 20};
    Product p3 = {3, "Headphones", 199.99, 30};
    Product p4 = {4, "Keyboard", 49.99, 50};
    Product p5 = {5, "Mouse", 29.99, 100};
    Product p6 = {6, "Shampoo", 10.99, 200};
    Product p7 = {7, "Soap", 5.99, 500};
    Product p8 = {8, "Tooth Paste", 9.99, 150};
    Product p9 = {9, "Tooth Brush", 2.99, 900};
    Product p10 = {10, "Charger", 23.99, 250};

    // Insert products into the AVL tree
    root = insert(root, p1);
    root = insert(root, p2);
    root = insert(root, p3);
    root = insert(root, p4);
    root = insert(root, p5);
    root = insert(root, p6);
    root = insert(root, p7);
    root = insert(root, p8);
    root = insert(root, p9);
    root = insert(root, p10);

    // Display available products
    cout << "\nAvailable Products:\n";
    displayProducts(root);

    // Create shopping cart and order queue
    ShoppingCart cart;
    Queue orderQueue;

    int choice;
    do {
        // Display menu options
        cout << "\n 1. Add item to cart";
        cout << "\n 2. Remove item from cart";
        cout << "\n 3. Display cart";
        cout << "\n 4. Modify cart item quantity";
        cout << "\n 5. Undo last action";
        cout << "\n 6. Place order";
        cout << "\n 7. Exit";
        cout << "\n Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1: {
            int id, qty;
            cout << "Enter product ID to add: ";
            cin >> id;
            cout << "Enter quantity: ";
            cin >> qty;

            Node* productNode = search(root, id);
            if (productNode && productNode->product.quantity >= qty) {
                cart.addItem(id, productNode->product.name, productNode->product.price, qty);
                productNode->product.quantity -= qty;  // Update inventory quantity
            } else {
                cout << "Product not available or insufficient stock.\n";
            }
            break;
        }
        case 2: {
            int id;
            cout << "Enter product ID to remove: ";
            cin >> id;
            cart.removeItem(id);
            break;
        }
        case 3:
            cart.displayCart();
            break;
        case 4: {
            int id, newQty;
            cout << "Enter product ID to modify: ";
            cin >> id;
            cout << "Enter new quantity: ";
            cin >> newQty;
            cart.modifyItem(id, newQty);
            break;
        }
        case 5:
            cart.undo();
            break;
        case 6: {
            cart.displayCart();

            // Calculate the total price of the cart
            double totalPrice = 0.0;
            CartNode* temp = cart.getHead(); // Use the getter method to access head
            while (temp) {
                totalPrice += temp->price * temp->quantity;
                temp = temp->next;
            }

            // Display the total price
            cout << "Total Price: $" << totalPrice << endl;

            orderQueue.enqueue(1); // Add order ID to the queue
            cout << "Order placed successfully!\n";
            break;
        }
        case 7:
            cout << "Exiting the program. Thank you for shopping with us!\n";
            break;
        default:
            cout << "Invalid choice! Please try again.\n";
            break;
        }
    } while (choice != 7);
     return 0;
}
