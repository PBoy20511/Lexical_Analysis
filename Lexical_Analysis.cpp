#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <stdio.h>

class Table {

public:

};

class HashTable: public Table {
    // table 5: symbol
    // table 6: literal
    // table 7: string


public:
    std::string table[99] ;

    int hashFunction( std::string token ) {
        int hashValue = 0 ;
        //std::cout << token << std::endl ;
        for ( int i = 0 ; i < token.size() ; i++ ) {
            // std::cout << token << std::endl ;
            //std::cout << token.at(i) << std::endl ;
            //std::cout << (int)token.at(i) ;
            hashValue = hashValue + token.at( i ) ;

        } // end for

        hashValue = hashValue % 100 ;
        //std::cout << hashValue << std::endl ;
        return hashValue ;
    }

    int hashIn( std::string token ) {
        //std::cout << token << std::endl ;
        int hashValue = hashFunction( token ) ;
        //std::cout << hashValue << std::endl ;
        bool isIn = false ;
        int probeCount = 0 ;

        while( !isIn && probeCount <= 100 ) {
            if( table[ hashValue ].empty() ) {
                table[ hashValue ] = token ;
                isIn = true ;
            }
            else if ( table[ hashValue ].compare( token ) == 0 ) {
                isIn = true ;
            }
            else {
                hashValue = ( hashValue + 1 ) % 100 ;
            }

            probeCount++ ;
        }

        if( isIn ) {
            //std::cout << hashValue << std::endl ;
            return hashValue ;
        }
        else {
            return -1 ;
        }

    } // end hashIn


}; // class HashTable

class DefaultTable: public Table {
    // for instruction, reg, pseudo ...
    std::vector< std::string > table ;

public:
    bool readTable() {
        std::cout << "Table file name: " ;
        std::string tableName;
        std::cin >> tableName ;

        return readTable( tableName ) ;

    }

    bool readTable( std::string tableName ) {
        std::cout << "Table file: " << tableName << std::endl ;

        std::fstream file ;
        file.open( tableName.c_str(), std::fstream::in | std::fstream::binary ) ;

        if ( !file.is_open() ) {
            return false ;
        }
        else {
            table.clear() ;

            std::string token = "" ;
            table.emplace_back( token ) ;
            while( file >> token ) {
                table.emplace_back( token ) ;
                //std::cout << table.back() << std::endl;
            }
        }

        file.close() ;
        return true ;
    }

    int searchStr( std::string token ) {
        // return index
        for( int i = 1 ; i < table.size() ; i++ ) {
            if ( iequals( token, table.at( i ) ) ) {
                return i ;
            }
        }
        return -1 ;
    }

    bool iequals( std::string a, std::string b) {
        if ( b.size() != a.size() ) {
            return false;
        }


        for ( int i = 0; i < a.size(); i++ ) {
            if ( tolower( a[i] ) != tolower( b[i] ) ) {
                return false;
            }
        }

        return true;
    }

}; //class DefaultTable

class TokenCoordinate{
    // token's table & index
public:
    int tableIndex ;
    int index ;

    TokenCoordinate(){
        tableIndex = -1 ;
        index = -1 ;
    }

    void clearData() {
        tableIndex = -1 ;
        index = -1 ;
    }
}; // class TokenCoordinate

class Analyzer {
    std::vector< HashTable > tables ; // 3 tables
    std::vector< DefaultTable > dTables ; // 4 tables
    std::vector< std::string > input ; // input content
    std::vector< std::vector< TokenCoordinate > > coordinates ;

public:
    bool readFile( ) {
        std::cout << "Source code file name: " ;
        std::string fileName;
        std::cin >> fileName ;
        return readFile( fileName ) ;
    }

    bool readFile( std::string fileName ) {
        // read input file
        std::fstream file ;
        file.open( fileName.c_str(), std::fstream::in ) ; // binary???

        if ( !file.is_open() ) {
            return false ;
        } // end if
        else {
            // get token
            char ch ;
            std::string token ;
            std::string line ; // a line of input
            std::vector< TokenCoordinate > lineCoordinate ; // token coordinate for 1 line
            TokenCoordinate coordinate ; // a coordinate for a token

            while( file.get( ch ) ) {
                //std::cout << ch << std::endl;
                if( isDelimiter( ch ) ) {
                    if( !token.empty() ) {
                        if( ch == '\'' ) {
                            coordinate.clearData() ;
                            locate( ch, coordinate ) ;
                            lineCoordinate.push_back( coordinate ) ;

                            if( token.at(0) == 'C' || token.at(0) == 'X' ) {
                                token += ch ;
                                line += ch ;
                                while( file.get( ch ) && ch != '\'' ) {
                                    token += ch ;
                                    line += ch ;
                                }

                                token += ch ;
                                line += ch ;

                                coordinate.clearData() ;
                                locate( token, coordinate ) ;
                                lineCoordinate.push_back( coordinate ) ;
                                token.clear() ;

                                coordinate.clearData() ;
                                locate( ch, coordinate ) ;
                                lineCoordinate.push_back( coordinate ) ;

                                file.get( ch ) ;
                            }

                        }
                        else {
                            coordinate.clearData() ;
                            locate( token, coordinate );
                            lineCoordinate.push_back( coordinate ) ;
                            token.clear() ;

                            coordinate.clearData() ;
                            locate( ch, coordinate ) ;
                            lineCoordinate.push_back( coordinate ) ;

                            if( ch == '.' ) {
                                line += ch ;
                                while( file.get( ch ) && ch != '\n' ) {
                                    line += ch ;
                                }

                            }
                        }

                    } // if !token.empty
                    else {
                        coordinate.clearData() ;
                        locate( ch, coordinate ) ;
                        lineCoordinate.push_back( coordinate ) ;

                        if( ch == '.' ) {
                            line += ch ;
                            while( file.get( ch ) && ch != '\n' ) {
                                line += ch ;
                            }

                        }

                    }
                }
                else if ( ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r' ) {
                    if ( !token.empty() ) {
                        coordinate.clearData() ;
                        locate( token, coordinate ) ;
                        lineCoordinate.push_back( coordinate ) ;
                        token.clear() ;
                    }
                }

                if ( ch != ' ' && ch != '\t' && ch != '\n' && ch != '\r' && !isDelimiter( ch ) ) {
                    token += ch ;
                }

                if ( ch == '\n' ) {
                    input.emplace_back( line ) ;
                    std::cout << line << std::endl ;
                    line.clear() ;

                    coordinates.push_back( lineCoordinate ) ;
                    for( int i = 0 ; i < lineCoordinate.size() ; i++) {
                        std::cout << "(" << lineCoordinate.at(i).tableIndex << ","
                                  << lineCoordinate.at(i).index << ")" ;
                    }
                    std::cout << std::endl ;
                    lineCoordinate.clear() ; //
                }
                else {
                    line += ch ;
                }

            } // while get ch

            if ( !token.empty() ) {
                coordinate.clearData() ;
                locate( token, coordinate ) ;
                lineCoordinate.push_back( coordinate ) ;
                token.clear() ;
            }

            input.emplace_back( line ) ;
            std::cout << line << std::endl ;
            line.clear() ;

            coordinates.push_back( lineCoordinate ) ;
            for( int i = 0 ; i < lineCoordinate.size() ; i++) {
                std::cout << "(" << lineCoordinate.at(i).tableIndex << ","
                          << lineCoordinate.at(i).index << ")" ;
            }

            std::cout << std::endl ;
            lineCoordinate.clear() ;

        } // end else

        file.close() ;
        return true ;
    } // readFile

    bool locate( char ch, TokenCoordinate& coordinate ) {
        std::string tempStr ;
        tempStr = ch ;
        locate( tempStr, coordinate ) ;
    } // locate

    bool locate( std::string token, TokenCoordinate& coordinate ) {
        // figure out which table & index belong to token
        // default table

        int index = -1 ;
        for( int i = 0 ; i < 4 ; i++ ) {
            index = dTables.at( i ).searchStr( token ) ;
            //std::cout << index << std::endl ;
            if( index != -1 ) {
                coordinate.tableIndex = i + 1 ;
                coordinate.index = index ;
                return true ;
            }

        }

        if ( index == -1 ) {
            //std::cout << token << std::endl ;
            // extra table
            if( token.size() >= 2 && token.at(1) == '\'' ) {
                if( token.at(0) == 'C') {
                    // string
                    coordinate.tableIndex = 7 ;
                    token.erase( 0,2 ) ; // C'
                    token.erase( token.size() - 1 ) ; // '
                    coordinate.index = tables.at( 2 ).hashIn( token ) ;
                }
                else if ( token.at(0) == 'X' ) {
                    //  literal
                    coordinate.tableIndex = 6 ;
                    token.erase( 0,2 ) ; // F'
                    token.erase( token.size() - 1 ) ; // '
                    coordinate.index = tables.at( 1 ).hashIn( token ) ;
                }

            }
            else if( '0' <= token.at(0) && token.at(0) <= '9' ) {
                // literal, no.2 in extra table
                coordinate.tableIndex = 6 ;
                //std::cout << token << std::endl ;
                coordinate.index = tables.at( 1 ).hashIn( token ) ;
            }
            else {
                // symbol, no.1 in extra table
                coordinate.tableIndex = 5 ;
                coordinate.index = tables.at( 0 ).hashIn( token ) ;
            }

        } // extra table

    }

    bool testDTable( std::string token ) {
        std::cout << dTables.at(1).searchStr( token ) ;

    }

    bool isDelimiter( char ch ) {
        std::string tempStr ;
        tempStr = ch ;
        return isDelimiter( tempStr ) ;
    }

    bool isDelimiter( std::string token ) {
        // check token with delimiter table to find out if it is a delimiter
        int index ;
        index = dTables.at( 3 ).searchStr( token ) ;
        if ( index == -1 ) {
            // -1 means not found in delimiter table
            return false ;
        }

        return true ;

    }

    bool readTables() {
        // read default table into program for reference
        tables.clear();
        dTables.clear() ;

        DefaultTable dTable ;

        std::cout << "Instruction table:" << std::endl ;
        while( !dTable.readTable() ) {
            std::cout << "Load table failed." << std::endl ;
        }
        dTables.emplace_back( dTable ) ;

        std::cout << std::endl << "Pseudo instruction table:" << std::endl ;
        while( !dTable.readTable() ) {
            std::cout << "Load table failed." << std::endl ;
        }
        dTables.emplace_back( dTable ) ;

        std::cout << std::endl << "Register table:" << std::endl ;
        while( !dTable.readTable() ) {
            std::cout << "Load table failed." << std::endl ;
        }
        dTables.emplace_back( dTable ) ;

        std::cout << std::endl << "Delimiter table:" << std::endl ;
        while( !dTable.readTable() ) {
            std::cout << "Load table failed." << std::endl ;
        }
        dTables.emplace_back( dTable ) ;

        HashTable hTable ;
        tables.emplace_back( hTable ) ;
        tables.emplace_back( hTable ) ;
        tables.emplace_back( hTable ) ;

    }

    void resultOutput() {
        // write analyzation result into a .txt file
        std::fstream file ;
        std::string fileName = "SIC_output.txt" ;
        file.open( fileName.c_str(), std::fstream::out | std::fstream::trunc ) ;
        if( file.is_open() ) {
            for ( int i = 0 ; i < input.size() ; i++ ) {
                file << input.at(i) << std::endl ;
                std::vector< TokenCoordinate > thisLine = coordinates.at( i ) ;
                for ( int j = 0 ; j < thisLine.size() ; j++ ) {
                    file << "(" << thisLine.at( j ).tableIndex << ","
                         << thisLine.at( j ).index << ")" ;
                }

                file << std::endl ;

            } // for input lines

            file.close() ;
        }

    }

    void testReadTable() {
        // automatically read tables & source file
        // not cautiously
        DefaultTable table ;
        table.readTable( "Table1.table" ) ;
        dTables.emplace_back( table ) ;
        // should initialize the temp table
        table.readTable( "Table2.table" ) ;
        dTables.emplace_back( table ) ;
        table.readTable( "Table3.table" ) ;
        dTables.emplace_back( table ) ;
        table.readTable( "Table4.table" ) ;
        dTables.emplace_back( table ) ;

        HashTable hashTable ;
        for ( int i = 0 ; i < 3 ; i ++ ) {
            tables.emplace_back( hashTable ) ;
        }

    }

}; // class Analyzer

int main() {
    std::cout << "<< Lexical Analysis >>" << std::endl
              << "( 1 ) Regular Mode" << std::endl
              << "( 2 ) Debug Mode" << std::endl
              << "( 0 ) Exit" << std::endl
              << "Mode: " ;
    std::string mode ;
    std::cin >> mode ;

    Analyzer analyzer ;
    while ( mode.compare("0") != 0 ) {
        if( mode.compare("1") == 0 ) {
            // manualy type table & file
            analyzer.readTables() ;
            // read source code
            if ( !analyzer.readFile() ) {
                std::cout << "Analyze file failed." << std::endl ;
            }
            else {
                analyzer.resultOutput() ;
            }

        }
        else if( mode.compare("2") == 0 ) {
            // automatically read table & file
            analyzer.testReadTable() ;
            analyzer.readFile("SIC_input.txt") ;
            analyzer.resultOutput() ;
        }

        std::cout << std::endl << "<< Lexical Analysis >>" << std::endl
                  << "( 1 ) Regular Mode" << std::endl
                  << "( 2 ) Debug Mode" << std::endl
                  << "( 0 ) Exit" << std::endl
                  << "Mode: " ;
        std::cin >> mode ;
    } // while running


    return 0;
}
