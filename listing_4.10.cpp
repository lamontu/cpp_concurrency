#include <future>
#include <set>

struct payload_type
{
};

struct outgoing_packet 
{
  payload_type payload;
  std::promise<bool> promise;
};

struct data_packet
{
  int id;
  payload_type payload;
};

struct network_connection
{
  data_packet incoming() const
  {
    return data_packet();
  }
  std::promise<payload_type>& get_promise(int) const
  {
    std::promise<payload_type> *prm = new std::promise<payload_type>();
    return *prm;
  }
  bool has_incoming_data() const
  {
    return true;
  }
  bool has_outgoing_data() const
  {
    return true;
  }
  outgoing_packet top_of_outgoing_queue() const
  {
    return outgoing_packet(); 
  }
  void send(const payload_type&) const
  {
  }

};

typedef std::set<network_connection> connection_set;
typedef std::set<network_connection>::iterator connection_iterator;

bool done(const connection_set &connections)
{
  return false;
}

void process_connections(connection_set& connections)
{
    while(!done(connections))
    {
        for(connection_iterator
                connection=connections.begin(),end=connections.end();
            connection!=end;
            ++connection)
        {
            if(connection->has_incoming_data())
            {
                data_packet data=connection->incoming();
                std::promise<payload_type>& p=
                    connection->get_promise(data.id);
                p.set_value(data.payload);
            }
            if(connection->has_outgoing_data())
            {
                outgoing_packet data=
                    connection->top_of_outgoing_queue();
                connection->send(data.payload);
                data.promise.set_value(true);
            }
        }
    }
}

int main()
{
  return 0;
}
