#include <stdlib.h>
#include <iostream>
#include <cctype>

#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>

using namespace std;
using namespace sql;

int main()
{
    //for demonstration only. never save your password in the code!
    const string server = "tcp://127.0.0.1:3306";   //localhost server
    const string username = "root";
    const string password = "GoodM@n121";

    Driver* driver;
    Connection* con;
    Statement* stmt;
    PreparedStatement* pstmt;

    try
    {
        driver = get_driver_instance();
        con = driver->connect(server, username, password);
    }
    catch (SQLException e)
    {
        cout << "Could not connect to server. Error message: " << e.what() << endl;
        system("pause");
        exit(1);
    }

    char Choice{ ' ' };
    a:
    cout << "Select one of the options below:\n";
    cout << "(A) to insert dummy data into the database\n";
    cout << "(B) to retrieve dummy data from the database\n";
    cout << "Your choice: ";
    cin >> Choice;

    Choice = toupper(Choice);

    switch (Choice)
    {
        case'A':
        {
            try
            {
                //please create database "quickstartdb" ahead of time
                con->setSchema("quickstartdb");

                stmt = con->createStatement();
                stmt->execute("DROP TABLE IF EXISTS inventory");
                cout << "Finished dropping table (if existed)" << endl;
                stmt->execute("CREATE TABLE inventory (id serial PRIMARY KEY, name VARCHAR(50), quantity INTEGER);");
                cout << "Finished creating table" << endl;
                delete stmt;

                pstmt = con->prepareStatement("INSERT INTO inventory(name, quantity) VALUES(?,?)");
                pstmt->setString(1, "banana");
                pstmt->setInt(2, 150);
                pstmt->execute();
                cout << "One row inserted." << endl;

                pstmt->setString(1, "orange");
                pstmt->setInt(2, 154);
                pstmt->execute();
                cout << "One row inserted." << endl;

                pstmt->setString(1, "apple");
                pstmt->setInt(2, 100);
                pstmt->execute();
                cout << "One row inserted." << endl;

                delete pstmt;
                delete con;
                system("pause");
            }
            catch (SQLException What)
            {
                cout << "Something went wrong, Error: " << What.what() << endl;
            }

            break;
        }
        case'B':
        {
            try
            {
                con->setSchema("quickstartdb");

                Statement* stmt = con->createStatement();
                ResultSet* res = stmt->executeQuery("SELECT * FROM inventory");
                while (res->next())
                {
                    int Quantity = res->getInt("quantity");
                    string Name = res->getString("name");
                    cout << "Name of the item: " << Name << endl;
                    cout << "Quantity of the item: " << Quantity << endl;
                }
                delete res;
                delete stmt;
            }
            catch (SQLException What)
            {
                cout << "Something went wrong, Error: " << What.what() << endl;
            }
            
            break;
        }
        default:
        {
            cout << "Invalid choice, Try again\n";
            goto a;
            break;
        }
    }
    
    return 0;
}