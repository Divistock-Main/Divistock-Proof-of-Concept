#include <eosio/eosio.hpp>
#include <eosio/contract.hpp>
#include <eosio/asset.hpp>

using namespace eosio;
using namespace std;
CONTRACT smartloan : public contract {
  public:
    using contract::contract;
    smartloan(eosio::name receiver, eosio::name code, datastream<const char*> ds):contract(receiver, code, ds),
    currency_symbol("DIVCOIN",0) {}  


 
 //Step 1: Create the loan
 //Here, we will initiate the loan.
 //We get the owner, currency and term.
     ACTION initiateloan(name owner,std::string stcurrency, int days) {
    //  require_auth( owner );
    check(stcurrency=="DIVCOIN", "Only DIVCOIN is accepted at the momment");
     
      loanstructb_index smartloan(get_first_receiver(), get_first_receiver().value);
      smartloan.emplace(owner, [&]( auto& row ) {
       row.id = smartloan.available_primary_key (); 
       row.days = days;
       row.stcurrency = stcurrency;
       row.owner = owner; 
      });
    }
    
  //Step 2: Fund the loan
  //User send in any amount of tokenized asset of DIVISTOCK coin.
  //If a transfer event is triggered on the eosio.token contract, we listen to it. 
  [[eosio::on_notify("eosio.token::transfer")]]
  void fundloan(name from, name to, asset quantity, std::string memo){
        //Removes all transfers  sent by us
    // Removes all transfer where the receier is not us
    if(from == get_self() || to != get_self()){
     return; 
    }
  check(quantity.amount>0, "Not enough coins ");
  check(quantity.symbol==currency_symbol, "Wrong coin");
  
    //get loan details table in our own contract
    //with scope of from to get your balane
    loandetails_index loandetails(get_self(),from.value);
    auto iterator = loandetails.find(currency_symbol.raw());
 
 
 //Save the quantity sent to our contract
      loandetails.emplace(get_self(),[&](auto &row){
        row.funds = quantity;
        //Send owner, X valueof loan.
        //Step 3: Fund the Sender 
        //User then gains any tokenized asset with the same value as their staked asset.
      });
 
  }
 
 
  private:
   const symbol currency_symbol;
   
    TABLE tableStruct {
      name key;
      std::string name;
    };
    TABLE loandetails{
     // int id;
      asset funds;
     //Primary key
     uint64_t primary_key() const {return funds.symbol.raw();}
    };
    
    //Send #days needed for loan.
    //Along with currency of loan
    TABLE loanstructb{
      int id;
      name owner;
      string stcurrency;
      int days;
      
     // symbol currency_symbol;
     
     //Primary key
     uint64_t primary_key() const { return id; }
    };
    
    typedef eosio::multi_index<"table"_n, tableStruct> table;
    typedef eosio::multi_index<"loanstructb"_n,loanstructb>loanstructb_index;
    typedef eosio::multi_index<"loandetails"_n,loandetails>loandetails_index;
};

 