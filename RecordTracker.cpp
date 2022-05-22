#include "RecordTracker.h"

records::records(string fN)
{
    fileName = fN;
}

void records::printMenu(){

    int choice;

    cout << "Welcome to the Record Tracker\n";

    do 
    {
        cout << "\n1. Add Route/Record\n";
        cout << "2. Delete Route/Record\n";
        cout << "3. View All Routes and Records\n";
        cout << "10. Quit Program\n\n";
        cout << "Enter your choice: ";

        cin >> choice;
        
        switch (choice)
        {
            case 1: addRecord();break;
            case 2: deleteRecord();break;
            case 3: viewRecords();break;
            case 10: break;

            default:
            cout << "The choice you entered has not been recognised\n";break;
        }

    }while (choice != 10);
}

void records::readFile()
{
    allRecords.clear();

    ifstream existingRecords;
    string recordLine = "";

    vector<string> oneRecordList; 

    existingRecords.open(fileName,ios::in);

    if (existingRecords.is_open())
    {
        while(getline(existingRecords,recordLine,'\n'))
        {
            if(recordLine.front() == '#')
            {
                oneRecordList.push_back(recordLine);
            }

            else if (recordLine.front() == '%')
            {
                oneRecordList.push_back(recordLine);
                allRecords.push_back(oneRecordList);
                //must clear VECTOR list once each route is complete
                oneRecordList.clear();
            }

            else
            {
                oneRecordList.push_back(recordLine);
            }
        }
    }

    else 
    {
        cout << "The file failed to open for reading\n";
    }

    existingRecords.close();
}

void records::addRecord()
{
    readFile();

    string routeChoice = ""; 
    string recordToAdd = "";
    string routeORrecord = "";
    string newRoute = "";
    int routeNumber = 1;
    int routeToChange = 999;

    ofstream outputFile;

    cout << "Route Names:\n";

    for (int i=0; i < (int)allRecords.size(); i++)
    {
        cout << routeNumber << ". " << allRecords[i][0] << endl;
        routeNumber += 1;
    }

    cout << "Enter 'RR' to add a route or 'rr' to add a record: ";
    getline(cin >> ws,routeORrecord);

    if (routeORrecord == "RR")
    {
        vector<string> newRouteVector;

        cout << "Enter the name of the new route: ";
        getline(cin >> ws,newRoute);

        newRoute = '#' + newRoute;
        newRouteVector.push_back(newRoute);
        newRouteVector.push_back("%");
        allRecords.push_back(newRouteVector);

        outputFile.open(fileName,ios::app);

        if (outputFile.is_open())
        {    
            for (int i =0; i < (int)newRouteVector.size();i++)
            outputFile << endl << newRouteVector[i];

            cout << "The new route has ben successfully added!!!!\n";

            newRouteVector.clear();
        }

        else {cout << "The file could not be opened in order to add new Route";}
        outputFile.close();

        removeBlankLines();

    }

    else if (routeORrecord == "rr")
    {
        cout << "Which route would you like to add to ?\n";
        getline(cin >> ws,routeChoice);

        routeChoice = '#' + routeChoice;

        for (int a=0;a < (int)allRecords.size(); a++)
        {
            if (routeChoice == allRecords[a][0]) //route records found
            {
                routeToChange = a;//location of route in vector
            }
        }

        if (routeToChange != 999)
        {
            cout << "Enter the date[dd/mm/yy] and time [mins.secs]\n";
            getline(cin >> ws,recordToAdd);

            //insert new information at the END of the SPECIFIC route vector BEFORE %
            allRecords[routeToChange].insert(allRecords[routeToChange].end()-1,recordToAdd);

            outputFile.open(fileName,ios::out);

            if (outputFile.is_open())
            {
                for (int i=0;i < (int)allRecords.size(); i++)
                {
                    for (int j=0;j < (int)allRecords[i].size(); j++)
                    {
                        outputFile << allRecords[i][j] << endl;
                    }
                }

                removeBlankLines();
            }

            else {cout << "The file could not be opened in order to add new record\n";}

            outputFile.close();
        }

        else
        {
            cout << "The route you entered does not exist\n";
        }


    }

}

void records::deleteRecord()
{
    readFile();

    string routeORrecord = "";
    string routeName = "";
    bool routeFound {false};

    vector<vector<string>>::iterator routeIterator;
    vector<string>::iterator recordIterator;

    cout << "Enter 'RR' to delete an ENTIRE route or 'rr' for an individual record: ";
    getline(cin >> ws,routeORrecord);

    if (routeORrecord == "RR")
    {
        for (routeIterator = allRecords.begin(); routeIterator != allRecords.end(); routeIterator++) 
        {
            cout << *routeIterator->begin() << endl;
        }

        cout << "Enter the name of the route you wish to delete: ";
        getline(cin >> ws,routeName);

        routeName = '#' + routeName;

        for (routeIterator = allRecords.begin(); routeIterator != allRecords.end(); routeIterator++) 
        {
            if (*routeIterator->begin() == routeName)
            {
                routeFound = {true};
                routeIterator->clear();
                cout << "The route has successfully been deleted!!!!" << endl;
                break;
            }
        }
        //send to file
        if (routeFound == true)
        {
            ofstream output(fileName,ios::out);

            for (int i=0;i < (int)allRecords.size();i++)
            {
                for( int j=0;j < (int)allRecords[i].size();j++)
                {
                    output << allRecords[i][j] << endl;
                }
            }

            removeBlankLines();
        }

        else if (routeFound == false)
        {
            cout << "The record you entered was not found\n";
        }
    }

    else if (routeORrecord == "rr")
    {
        bool recordFound = {false};
        string recordToDelete = "";

        cout << "Enter the route name: ";
        getline(cin >> ws,routeName);
        routeName = '#'+routeName; 

        cout << "Enter the date you wish to remove: ";
        getline(cin >> ws,recordToDelete);

        for (routeIterator = allRecords.begin(); routeIterator != allRecords.end(); routeIterator++) 
        {
            if (*routeIterator->begin() == routeName)
            {
                routeFound = {true};
            }

            for (recordIterator = routeIterator->begin(); recordIterator != routeIterator->end(); recordIterator++) 
            {
                if (*recordIterator == recordToDelete && routeFound == true)
                {
                    //If record matches input AND the route was found
                    recordFound = {true};
                    recordIterator->erase();
                    break;//exit for loop once record is erased
                }
            }
        }

        //send to file
        if (routeFound == true && recordFound == true)
        {
            ofstream output(fileName,ios::out);

            for (int i=0;i < (int)allRecords.size();i++)
            {
                for( int j=0;j < (int)allRecords[i].size();j++)
                {
                    output << allRecords[i][j] << endl;
                }
            }

            removeBlankLines();
        }

        if (routeFound == false)
        {
            cout <<  "The route you entered was not located\n";
        }

        if (routeFound == true && recordFound == false )
        {
            cout << "The record you entered was not located\n";
        }

    } 

    else 
    {
        cout << "You entered an invalid choice\n";
    } 
}

void records::removeBlankLines()
{
    ifstream fin(fileName);    
  
    ofstream fout;                
    fout.open("temp.txt", ios::out);
    
    if (fout.is_open())
    {
        string str = "";
        while(getline(fin,str))
        { 
            while (str.length()==0 ) 
            getline(fin,str);   
        
            fout<<str<<endl;
        }
    }

    fout.close();  
    fin.close();  
    remove(fileName.c_str());        
    rename("temp.txt", fileName.c_str());
}

void records::viewRecords()
{
    readFile();

    for (int i=0;i < (int)allRecords.size(); i++)
    {
        for (int j=0;j < (int)allRecords[i].size(); j++)
        {
            if (allRecords[i][j].front() == '#' )
            {
                allRecords[i][j].erase(allRecords[i][j].begin());
                cout << allRecords[i][j] << endl;
            }

            else if  (allRecords[i][j] == "%")
            {
                cout << endl;
            }

            else
            {cout << allRecords[i][j] << endl;}
        }
    }

    cout << endl;

    char return2Menu;
    cout << "Enter any key to return to the menu: ";
    cin >> return2Menu;
}
