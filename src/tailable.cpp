#include <cstdlib>
#include <iostream>
#include <string>
#include "mongo/client/dbclient.h"

namespace test {
  using namespace std;
  using namespace mongo;

  class Tailer
  {
  public:
    Tailer( DBClientBase &conn, const string &ns ) : __conn( conn ), __ns( ns ) {}

    void operator() ()
    {
      BSONElement last_id = minKey.firstElement();
      Query query = Query().hint( BSON( "$natural" << 1 ) );
      while ( 1 ) {
        auto cursor( __conn.query( __ns, query, 0, 0, 0,
                                   QueryOption_CursorTailable | QueryOption_AwaitData ) );
        while ( 1 ) {
          if ( ! cursor->more() ) {
            if ( cursor->isDead() )
              break;
            else
              continue;
          }
          BSONObj obj = cursor->next();
          last_id = obj["_id"];
          cout << obj.toString() << endl;
        } // wend
        query = QUERY( "_id" << GT << last_id ).hint( BSON( "$natural" << 1 ) );
      } // wend
    }

  private:
    DBClientBase &__conn;
    const string &__ns;
  };
}

int main( /*int argc, char *argv[]*/ )
{
  try {
    mongo::DBClientConnection conn;
    conn.connect( "localhost" );
    test::Tailer( conn, "tailable.objects" )();
    return EXIT_SUCCESS;
  } catch( const mongo::DBException &e ) {
    std::cout << "caught " << e.what() << std::endl;
    return EXIT_FAILURE;
  }
}
