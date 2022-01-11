#import "uuid"

var guid = uuid.uuid4();
println(guid);

var guid2 = new uuid::UUID(guid);
println(guid2.tostring());