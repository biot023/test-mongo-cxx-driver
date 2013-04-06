#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include "mongo/client/dbclient.h"

namespace runner {
  using namespace std;
  using namespace mongo;
  
  class Runner
  { 
  public:
    void operator() () const
    {
      vector<string> names( { "Peter", "Michael", "Satan", "Horsey", "Genevieve" } );
      DBClientConnection conn;
      conn.connect( "localhost" );
      for ( size_t i( 0 ) ; i < 100 ; ++ i )
        conn.insert( "tutorial.people",
                     BSON( GENOID
                           << "name"       << names[i % 5]
                           << "age"        << ( 31 + static_cast<int>( i % 5 ) )
                           << "created_at" << DATENOW ) );
      cout << "Count of people: " << conn.count( "tutorial.people" ) << endl;
      auto cursor( conn.query( "tutorial.people", BSONObj() ) );
      while ( cursor->more() )
        cout << cursor->next().toString() << endl;
    }
  };  
}

int main() {
  try {
    runner::Runner()();
    std::cout << "All good." << std::endl;
    return EXIT_SUCCESS;
  } catch( const mongo::DBException &e ) {
    std::cerr << "caught " << e.what() << std::endl;
    return EXIT_FAILURE;
  }
}
