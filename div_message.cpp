/* This send_message contract can send messsages to any account using the 
*require_recipient().
*This function will introduce an inline action that communicates the parameters 
*passed to the action to the desired recipient.
*/



#include <eosiolib/eosio.hpp>
#include <string>

using eosio::contract;
using std::string;

class div_messages : contract {
    public:
        send_messages(account_name account) : contract(account) {};

        void send(account_name account, string message) {
            require_recipient(account);
        }
};

EOSIO_ABI(send_messages, (send));