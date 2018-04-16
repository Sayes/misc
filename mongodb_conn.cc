/*
 *
 */

#include <stdio.h>
#include <bsoncxx/builder/basic/array.hpp>
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>
#include <iostream>
#include <mongocxx/client.hpp>
#include <mongocxx/cursor.hpp>
#include <mongocxx/exception/query_exception.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/options/find.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>

int main(int argc, char* argv[]) {
  int result = -1;
  do {
    try {
      mongocxx::uri uri_("mongodb://localhost:27017");
      mongocxx::instance inst_;
      mongocxx::client client_;
      mongocxx::database db_;

      client_ = mongocxx::client(uri_);
      db_ = client_["local"];

      mongocxx::options::find opts;
      bsoncxx::builder::stream::document order_builder;
      order_builder << "id_" << -1;
      opts.sort(order_builder.view());
      bsoncxx::builder::stream::document filter;

      mongocxx::cursor cursor_ = db_["startup_log"].find(filter.view(), opts);
      mongocxx::cursor::iterator it = cursor_.begin();

      int i = 0;
      for (; it != cursor_.end(); ++it) {
        bsoncxx::document::view view = *it;
        bsoncxx::document::element element{view["pid"]};
        printf("%d\n", i++);
      }

      result = 0;
    } catch (mongocxx::query_exception e) {
    }
  } while (0);
  return result;
}