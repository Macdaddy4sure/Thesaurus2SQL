/*
    Copyright(C) 2022 Tyler Crockett | Macdaddy4sure.com

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissionsand
    limitations under the License.
*/

#include <iostream>
#include <string>
#include <mysql.h>

using namespace std;

string ParseThesaurus(string input_file);
void Thesaurus2SQL(string word, string pos, string* synonyms);
void QueryDatabase(string database, string table, string dictionary_word, string dictionary_pos, string dictionary_definition, string thesaurus_word, string thesaurus_pos, string* thesaurus_synonyms);
bool wordExists(string word);

string ParseThesaurus(string input_file)
{
    string word;
    string pos;
    string synonyms[10];
    int num_synonyms = 0;
    fstream file;

    file.open(input_file.c_str());

    if (file.is_open())
    {
        while (getline(file, input))
        {
            word.clear();
            pos.clear();

            for (int a = 0; a <= input.length(); a++)
            {
                // Get the word
                if (input[a] == '{' && input[a + 1] == '\"' && input[a + 2] == 'w' && input[a + 3] == 'o' && input[a + 4] == 'r' && input[a + 5] == 'd' && input[a + 6] == '\"' && input[a + 7] == ':' && input[a + 8] == ' ' && input[a + 9] == '\"')
                {
                    for (int b = 10; b <= input.length(); b++)
                    {
                        if (input[b] == '\"' && input[b + 1] == ',')
                        {
                            break;
                        }
                        else
                        {
                            word += input[b];
                        }
                    }
                }

                // Get the part of speech
                if (input[a] == '\"' && input[a + 1] == 'p' && input[a + 2] == 'o' && input[a + 3] == 's' && input[a + 4] == '\"' && input[a + 5] == ':' && input[a + 6] == ' ' && input[a + 7] == '\"')
                {
                    for (int b = 8; b <= input.length(); b++)
                    {
                        if (input[b] == '\"' && input[b + 1] == ',')
                        {
                            break;
                        }
                        else
                        {
                            pos += input[b];
                        }
                    }
                }

                // Get the synonyms
                if (input[a] == '\"' && input[a + 1] == 's' && input[a + 2] == 'y' && input[a + 3] == 'n' && input[a + 4] == 'o' && input[a + 5] == 'n' && input[a + 6] == 'y' && input[a + 7] == 'm' && input[a + 8] == 's' && input[a + 9] == '\"' && input[a + 10] == ':' && input[a + 11] == ' ' && input[a + 12] == '[' && input[a + 13] == '\"')
                {
                    for (int b = 8; b <= input.length(); b++)
                    {
                        if (input[b] == '\"' && input[b + 1] == ',')
                        {
                            num_synonyms++;
                            break;
                        }
                        else if (input[b] == '\"' && input[b + 1] == ']' && input[b + 2] == '}')
                        {
                            break;
                        }
                        else
                        {
                            synonyms[num_synonyms] += input[b];
                        }
                    }
                }
            }

            Thesaurus2SQL(word, pos, synonyms);
        }
    }
}

// Purpose: Add one line from the thesaurus to the dictionary database
// 1. Print the line from the thesaurus
//      a. Word
//      b. Part of Speech
//      c. Synonyms
// 2. Get identical lines from the dictionary
//      a. Display all information from the dictionary for a given word
// 3. Based on the current line of the thesaurus and the number of dictionary definitions, make slections for each dictionary definition
void Thesaurus2SQL(string word, string pos, string* synonyms)
{
    MYSQL* conn;
    MYSQL_ROW row;
    MYSQL_RES* result;
    string mysql_username = "root";
    string mysql_password = "Anaheim228%";
    string mysql_database = "dictionary";
    string dictionary_table = "entries";
    string dictionary_word[5];
    string dictionary_pos[5];
    string dictionary_definition[5];
    string input;
    bool found = false;
    int number_selection = 1;
    string sql1;

    conn = mysql_init(0);
    conn = mysql_real_connect(conn, "127.0.0.1", mysql_username.c_str(), mysql_password.c_str(), mysql_database.c_str(), 3306, NULL, 0);

    if (conn)
    {
        sql1 = "SELECT * FROM ";
        sql1 += dictionary_table;
        /*cout << "SQL1: " << sql1 << endl;*/
        mysql_query(conn, sql1.c_str());
        result = mysql_store_result(conn);

        while (row = mysql_fetch_row(result))
        {
            if (word == row[0])
            {
                found = true;

                if (number_selection == 1)
                {
                    cout << "Thesaurus: " << endl;
                    cout << "Word: " << word << " POS: " << pos << " synonym[0]: " << synonyms[0];
                    if (synonyms[1] != "")
                        cout << " synonyms[1]: " << synonyms[1];
                    if (synonyms[2] != "")
                        cout << " synonyms[2]: " << synonyms[2];
                    if (synonyms[3] != "")
                        cout << " synonyms[3]: " << synonyms[3];
                    if (synonyms[4] != "")
                        cout << " synonyms[4]: " << synonyms[4];
                    if (synonyms[5] != "")
                        cout << " synonyms[5]: " << synonyms[5];
                    if (synonyms[6] != "")
                        cout << " synonyms[6]: " << synonyms[6];
                    if (synonyms[7] != "")
                        cout << " synonyms[7]: " << synonyms[7];
                    if (synonyms[8] != "")
                        cout << " synonyms[8]: " << synonyms[8];
                    if (synonyms[9] != "")
                        cout << " synonyms[9]: " << synonyms[9];
                    else
                        cout << endl;

                    cout << endl;
                    cout << "Dictionary: " << endl;
                    cout << number_selection << ". " << row[0] << " POS: " << row[1] << " Definition: " << row[2] << endl;
                    number_selection++;
                }
                else if (number_selection > 1)
                {
                    cout << number_selection << ". " << row[0] << " POS: " << row[1] << " Definition: " << row[2] << endl;
                    number_selection++;
                }
            }
            else if (word != row[0] && found)
            {
                bool boolean = false;

                while (!boolean)
                {
                    cout << endl;
                    cout << "Your Selection: ";
                    getline(cin, input);

                    if (input != "")
                    {
                        QueryDatabase(mysql_database, table, dictionary_word, dictionary_pos, dictionary_definition, thesaurus_word, thesaurus_pos, thesaurus_synonyms);
                        number_selection = 1;
                    }
                }
            }
        }
    }
}

void CreateTable()
{
    MYSQL* conn;
    MYSQL_ROW row;
    string mysql_database = "dictionary";
    string mysql_username = "root";
    string mysql_password = "Anaheim228%";
    string table = "entries3";
    string input;
    string sql1;

    conn = mysql_init(0);
    conn = mysql_real_connect(conn, "127.0.0.1", mysql_username.c_str(), mysql_password.c_str(), mysql_database.c_str(), 3306, NULL, 0);

    if (conn)
    {
        sql1 = "CREATE TABLE ";
        sql1 += table;
        sql1 += "(word TEXT, wordtype TEXT, definition TEXT, synonyms TEXT);\"";
        mysql_query(conn, sql1.c_str());
    }
}

void QueryDatabase(string mysql_database, string table, string dictionary_word, string dictionary_pos, string dictionary_definition, string thesaurus_word, string thesaurus_pos, string* thesaurus_synonyms)
{
    MYSQL* conn;
    MYSQL_ROW row;
    MYSQL_RES* result;
    string mysql_username = "root";
    string mysql_password = "Anaheim228%";
    string input;
    string sql1;

    conn = mysql_init(0);
    conn = mysql_real_connect(conn, "127.0.0.1", mysql_username.c_str(), mysql_password.c_str(), mysql_database.c_str(), 3306, NULL, 0);

    if (conn)
    {
        sql1 = "INSERT INTO entries3";
        sql1 += table;
        sql1 += "(word, wordtype, definition, synonyms) VALUES(\"";
        sql1 += word;
        sql1 += "\", \"";
        sql1 += wordtype;
        sql1 += "\", \"";
        sql1 += definition;
        sql1 += "\", \"";
        for (int x = 0; x <= 5; x++)
        {
            if (synonyms[x] == "" && synonyms[x + 1] == "")
            {
                sql1 += synonyms[x];
                sql1 += ", ";
            }
            else if (synonyms[x] == "" && synonyms[x + 1] != "")
            {
                sql1 += synonyms[x];
                sql1 += ");\"";
            }
        }

        /*cout << "SQL1: " << sql1 << endl;*/
        mysql_query(conn, sql1.c_str());
    }
}

bool wordExists(string word)
{
    MYSQL* conn;
    MYSQL_ROW row;
    MYSQL_RES* result;
    string mysql_username = "root";
    string mysql_password = "Anaheim228%";
    string dictionary_table = "entries";
    string word_temp;

    conn = mysql_init(0);
    conn = mysql_real_connect(conn, "127.0.0.1", mysql_username.c_str(), mysql_password.c_str(), mysql_database.c_str(), 3306, NULL, 0);

    if (conn)
    {
        sql1 = "SELECT * FROM ";
        sql1 += dictionary_table;
        /*cout << "SQL1: " << sql1 << endl;*/
        mysql_query(conn, sql1.c_str());
        result = mysql_store_result(conn);

        while (row = mysql_fetch_row(result))
        {
            word_temp = toLowerWord(row[0]);
            word = toLowerWord(word);

            if (word == word_temp)
            {
                return true;
            }
        }
    }
}

string toLowerWord(string word)
{
    string temp;

    for (int x = 0; x <= word.length(); x++)
    {
        temp += tolower(word[x]);
    }

    return temp;
}