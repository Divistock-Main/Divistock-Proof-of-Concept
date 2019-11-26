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
 //Here, we will initiate the loan.gg
 //We get the owner, currency and term.
    // ACTION initiateloan(name owner,std::string stcurrency, int days) {
     ACTION initiateloan(name owner,asset stcurrency, int days) {
    //  require_auth( owner );
    //Not Needed 
    //check(stcurrency=="DIVCOIN", "Only DIVCOIN is accepted at the momment");
//Check the the user initiate a loan with the correct currency.

            check_sent_coin(owner,stcurrency);


      loanstructc_index smartloan(get_first_receiver(), get_first_receiver().value);
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
  //check(quantity.symbol==currency_symbol, "Wrong coin");
    check_sent_coin(from,quantity);
  

    tradeditems_index tradeditems(get_self(),get_self().value); 
    auto tradedCur = tradeditems.find(quantity.symbol.raw());

    asset valRaw = tradedCur->rawvalue;
    asset valDiv = tradedCur->valuediv;


    //get loan details table in our own contract
    //with scope of from to get your balane
    loandetails_index loandetailsa(get_self(),from.value);
    auto iterator = loandetailsa.find(quantity.symbol.raw());

    //Save the quantity sent to our contract
      loandetailsa.emplace(get_self(),[&](auto &row){
        row.id = loandetailsa.available_primary_key (); 
        row.funds = quantity;
        //Send owner, X valueof loan.
        //Step 3: Fund the Sender 
        //User then gains any tokenized asset with the same value as their staked asset.
        //Look in the  instradeditemtable, and mmultiply the div coin value by the amount that they went
        // and return that in divcoin
        //1 appcoin
        //3 divcoin
        // 1 appcoin = 3 divcoin.

        //If I pass in 2 app coins, I should get 6 divcoin
       asset new_quantity = quantity.amount * valDiv;// quantity;
      // new_quantity *= 2;

       //Make sure payments can only be made
       // in one token.
       
        action(permission_level{_self, "active"_n}, "eosio.token"_n, "transfer"_n,
        make_tuple(_self, from, new_quantity, std::string("Happy 10x!"))).send();


      });
 
  }
 
 //Step 3 : Secure the loan.
 //A portion of what was sent to the contract can be used to crete security for the loan. 
 
      ACTION secureloan(name owner,asset quantity) {
        
      require_auth( owner );
      check_coin_balance(owner,quantity);
      reduce_coin_balance(owner,quantity);
     // check(stcurrency=="DIVCOIN", "Only DIVCOIN is accepted at the momment");
      loansecure_index secureloan(get_first_receiver(), get_first_receiver().value);
      secureloan.emplace(get_self(), [&]( auto& row ) {
      row.id = secureloan.available_primary_key (); 
      row.funds = quantity;
 
      });
    }
    
   //    loandetails_index loandetails(get_self(),from.value);
   // auto iterator = loandetails.find(currency_symbol.raw());


    
    //Admin Function 
 
 //TODO - Change those assets to strings and set a string Pk
    //ACTION instradeitem(asset rawvalue,asset valuediv,string note){
    ACTION instradeitem(asset rawvalue,asset valuediv,string note){
     
    tradeditems_index tradeditems(get_self(),get_self().value);

 //auto tradedcurrency = tradeditems.get_index<"tradeditems"_n>();//(rawvalue.symbol.code().raw());

  auto iterator = tradeditems.find(rawvalue.symbol.raw());
  //While the iterator is not at the END/ bottom of list
  //Means there are
  //while(iterator != tradedcurrency.end()){
  if(iterator == tradeditems.end()){  // If we got to the end of the table
      tradeditems.emplace(get_self(), [&](auto& row){
      row.id=tradeditems.available_primary_key();
      row.rawvalue = rawvalue;
      row.valuediv = valuediv;
      row.note=note;

   }
  );}
  else{
      eosio::print ("This already exist");
     }
  }
 

 
  private:
   const symbol currency_symbol;
   
 
   //This table holds the value of items we will be trading.
   TABLE tradeditems{
   int id;
   asset rawvalue;
   asset valuediv;
   std:: string note;
  //Primary key is symbol of rawValue
  uint64_t primary_key() const {return rawvalue.symbol.raw();}

   };
 
    TABLE tableStruct {
      name key;
      std::string name;

    };
    
    //Holds value of what was sent to the Smart Contract.
    //cleos get table inline bob loandetails- Shows what bob has sent
    TABLE loandetailsa{
      int id;
      asset funds;
     //Primary key
     uint64_t primary_key() const {return id;}
    };
    
    //Send #days needed for loan.
    //Along with currency of loan
    //cleos get table inline inline loanstructc
    //Shows all loans. 
    TABLE loanstructc{
      int id;
      name owner;
      asset stcurrency;
      int days;
      
     // symbol currency_symbol;
     
     //Primary key
     uint64_t primary_key() const { return id; }
    };
    
    //Table for Step 3:
    //Loan Security Table
    //Value individuals have sent as security for a loan.
        TABLE loansecure{
      int id;
      asset funds;
     //Primary key
    uint64_t primary_key() const { return id; }
    // uint64_t primary_key() const {return funds.symbol.raw();}
    };
    
    
    typedef eosio::multi_index<"table"_n, tableStruct> table;
    typedef eosio::multi_index<"loanstructc"_n,loanstructc>loanstructc_index;
    typedef eosio::multi_index<"loandetailsa"_n,loandetailsa>loandetails_index;
    typedef eosio::multi_index<"loansecure"_n,loansecure>loansecure_index;
    typedef eosio::multi_index<"tradeditems"_n,tradeditems>tradeditems_index;
    

    //Functions 

    void check_sent_coin(name owner,asset quantity){
    tradeditems_index tradeditems(get_self(),get_self().value);
     auto iterator = tradeditems.get(quantity.symbol.raw(),"This Coin is not traded");

     
     

    }
    void check_coin_balance(name owner,asset quantity){
      loandetails_index loandetails(get_self(),owner.value);

      auto row = loandetails.get(quantity.symbol.raw(),"No balance");
      check(row.funds.amount >=quantity.amount,"Not enough coins");
      
      
    }
        void reduce_coin_balance(name owner,asset quantity){
      loandetails_index loandetails(get_self(),owner.value);
      auto iterator = loandetails.find(quantity.symbol.raw());
      if (iterator != loandetails.end()){
        loandetails.modify(iterator,get_self(),[&](auto &row){
          row.funds.set_amount(row.funds.amount-quantity.amount);
        });
      }
      
      
    }
    
};

 