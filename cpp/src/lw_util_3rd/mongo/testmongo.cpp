#include <iostream>

#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>

#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>

#include <bsoncxx/builder/basic/array.hpp>
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>
#include <bsoncxx/types.hpp>

using namespace bsoncxx;

void test()
{
	mongocxx::instance inst{};
	mongocxx::client conn{ mongocxx::uri{} };

	bsoncxx::builder::stream::document document{};

	auto collection = conn["testdb"]["testcollection"];
	document << "hello" << "world";

	collection.insert_one(document.view());
	auto cursor = collection.find({});

	for (auto&& doc : cursor) {
		std::cout << bsoncxx::to_json(doc) << std::endl;
	}
}

int test2(int, char**) {
	// bsoncxx::builder::basic presents a BSON-construction interface familiar to users of the
	// server's
	// BSON library or the Java driver.

	// basic::document builds a BSON document.
	auto doc = builder::basic::document{};
	// basic::array builds a BSON array
	auto arr = builder::basic::array{};

	// We append key-value pairs to a document using the kvp helper.
	using bsoncxx::builder::basic::kvp;

	doc.append(
		kvp("foo", "bar"));  // string literal value will be converted to b_utf8 automatically
	doc.append(kvp("baz", types::b_bool{ false }));
	doc.append(kvp("garply", types::b_double{ 3.14159 }));

	// We can also pass a variable number of keys to append.
	doc.append(kvp("a key", "a value"),
		kvp("another key", "another value"),
		kvp("moar keys", "moar values"));

	// Appending to arrays is simple, just append one or more bson values.
	arr.append("hello");
	arr.append(false, types::b_bool{ true }, types::b_double{ 1.234 });

	// If we want to create a subdocument, we can pass lambda as a value with a sub_document
	// argument.

	// When append is executed, the builder will start a subdocument, call the lambda with itself
	// as a parameter, which appends the keys in-place.
	// After the lambda returns, the builder will end the subdocument.

	using bsoncxx::builder::basic::sub_document;
	using bsoncxx::builder::basic::sub_array;

	doc.append(kvp("subdocument key",
		[](sub_document subdoc) {
		subdoc.append(kvp("subdoc key", "subdoc value"),
			kvp("another subdoc key", types::b_int64{ 1212 }));
	}),
		kvp("subarray key", [](sub_array subarr) {
		// subarrays work similarly
		subarr.append(1,
			types::b_bool{ false },
			"hello",
			5,
			[](sub_document subdoc) {
			// nesting works too!
			subdoc.append(kvp("such", "nesting"), kvp("much", "recurse"));
		});
	}));

	// We can get a view of the resulting bson by calling view()
	auto v = doc.view();

	// Use 'v' so we don't get compiler warnings.
	return v.empty() ? EXIT_FAILURE : EXIT_SUCCESS;
}