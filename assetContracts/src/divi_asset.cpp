//#include <eosio/eosio.hpp>
//#include <eosio.asset.hpp>
#include <eosio.msig>
#include <divi_asset.hpp>

using namespace eosio;
using std::string;

class[[eosio::contract]] divi_asset : public eosio::contract
{
  public:
	using contract::contract;

    [[eosio::action]] void divi_asset::createAsset(name issuer, asset maximum_assets, string cert_number, double quantity)
    {
        //Ensure only the issuer trying to create the asset can only call this function
        require_auth(_issuer);

        //Get the name of our user contract so we can ask it to validate the user.
        settings_table config(_self, _self.value);
        Config existing_config = config.get();

        // This prepares the call using the action wrapper we defined in the user contract.
		user::validate_action validate(existing_config.user_contract, {get_self(), "active"_n});

        // This does the call as an inline action, passing username as a parameter to the action
		validate.send(username);                     
	                                   
	    //Check to see if the asset(s) is validated and tokenizable.
	    if asset > 0 and quantity > 0 then  "The asset and/or quantity must be larger than zero".
		    if  mainnet.cert_number.isValid() "The Asset provided is invalid. Please resubmit to evaluate the following asset"
			    _Exit(0)

			
	    //check for invalid values
	    auto sym = maximum_assets.symbol;
	    check(sym.is_valid(), "invalid symbol name");
	    check(maximum_assets.is_valid(), "invalid supply");
	    check(maximum_assets.amount > 0, "maximum_assets-supply must be positive");


	    //check for already existing symbol
	    stats statstable(_self, sym.code().raw());
	    auto existing = statstable.find(sym.code().raw());
	    check(existing == statstable.end(), "divi_asset with symbol already exists");


	    //add new digitized asset to table
	    statstable.emplace(_self, [&](auto& s) {
		s.supply.symbol = maximum_assets.symbol;
		s.max_supply = maximum_assets;
		s.issuer = issuer;

        });

    }

        [[eosio::action]] void divi_asset::issue(name to, asset maximum_assets, string cert_number) 
        
    {
	auto sym = maximum_assets.symbol; //check to see if symbol is valid and memo does not exceed 256 bytes
	check(sym.is_valid(), "invalid symbol name");
	check(cert_number.size() <= 256, "memo has more than 256 bytes");

	stats statstable(_self, sym.code().raw()); //check if divi_asset with symbol name exist, and get iterator_
	auto existing = statstable.find(sym.code().raw());
	check(existing != statstable.end(), "divi_asset with symbol does not exist, create divi_asset before issue");
	const auto& st = *existing;

	require_auth(st.issuer); //check for authority of issuer and valid
	check(maximum_assets.is_valid(), "invalid quantity");
	check(maximum_assets.amount > 0, "must issue positive quantity");

	check(maximum_assets.symbol == st.supply.symbol, "symbol precision mismatch");

    }
	
    //Contract to digitize Fungible assets
	name SIMPLEASSETSCONTRACT = "simpleassets"_n;

	name author = get_self();
	name category = "stock" //add divasset to the divi_asset contract to track types of divi_assetized assets.
		name owner = "owner" _n;
		string idata = { };
		string mdata = "{\"asset_type\":\"stock\", \"asset_value\": 130.00, \"asset_owner\": \"Jane Doe\" }";
		action createAsset = action(
			permission_level{ author, "active"_n },
			SIMPLEASSETSCONTRACT,
			"create"_n,
			std::make_tuple(category, owner, idata, mdata, 0)
		);
        createAsset.send();

	//modify records
	statstable.modify(st, same_payer, [&](auto& s) {
		s.supply += maximum_assets;
		if (s.supply > s.max_supply) {
			s.max_supply = s.supply;
		}
	});
	add_balance(st.issuer, maximum_assets, st.issuer);
	if (to != st.issuer) {
		SEND_INLINE_ACTION(*this, transfer, { {st.issuer, "active"_n} }, { st.issuer, to, maximum_assets, cert_number });
	}
}
//Make note of utility functions transfer and balance.
void divi_asset::transfer(name from, name to, asset add_balance(st.issuer, maximum_assets, st.issuer);
, string memo) 
{
	check(from != to, "cannot transfer to self");
	require_auth(from);
	check(is_account(to), "to account does not exist");
	auto sym = maximum_assets.symbol.code();
	stats statstable(_self, sym.raw());
	const auto& st = statstable.get(sym.raw());

	require_recipient(from);
	require_recipient(to);

	check( quantity.is_valid(), "invalid quantity");
	check(quantuty.amount > 0, "must transfer positive quantity");
	check(quantity.symbol == st.supply.symbol, "symbol precision mismatch");
	check(memo.size() <= 256, "memo has more than 256 bytes");

	auto payer = has_auth(to) ? to : from;

	sub_balance(from,quantity);
	add_balance(to, quantity, payer);
}

void divi_asset::sub_balance(name owner, asset value) {
	accounts from_acnts(_self, owner.value);

	const auto& from = from_acnts.get(value.symbol.code().raw(), "no balance object found");
	check(from.balance.amount >= value.amount, "overdrawn balance");
	if (from.balance.amount == value.amount) {
		from_acnts.erase(from);
	}
	else {
		from_acnts.modify(from, owner, [&](auto& a) {
			a.balance -= value;
		});
	}
}
void divi_asset::add_balance(name owner, asset value, name ram_payer) {
	accounts to_acnts(_self, owner.value);
	auto to = to_acnts.find(value.symbol.code().raw());
	if (to == to_acnts.end()) {
		to_acnts.emplace(ram_payer, [&](auto& a) {
			a.balance =value;
		});
	}
	else {
		to_acnts.modify(to, same_payer, [&](auto& a) {
			a.balance += value;
		});
	}
}
///Whitelist Users_Address
void divi_asset::whitelist(name owner string wallet_address boolean status){
	//check if user is valid
   if isvalid(req_auth(_self)) = true{
	   //add user to whitelist table 
	   statstable.emplace(_self, [&](auto& whitelist) {
		   whitelist.name = maximum_assets.symbol;
		   whitelist.wallet_address = wallet_address
		   whtielist.owner = issuer;
	   }
   else
   {
	   if issuer.isValid = false{ cout << "Please submit a user contract  for elgibility" }
   }
	})
    //divi_assetize assets it exists.
		void divi_assetize(name issuer asset maximum_assets) {
		 re
			 q_auth(_self)
		 existing = statstable.find.sym.raw()
		 if existing != statstable.end() {
			//Create the new asset and the associated
			 createAsset.send(asset_maximum)
				 check(issue{ owner, asset_maximum })
		 }
		 else {
			 check(issue{ owner, asset_maximum })
		   
		 }
	}
private:
TABLE account{
	  asset       balance;
	  uint64_t primary_key()const { return balance.symbol.code().raw(); }
};
TABLE currency_stats{
	  asset       supply;
	  asset       max_supply;
	  name        issuer;
	  uint64_t primary_key()const { return supply.symbol.code().raw(); }
};
TABLE blacklist_table{
	  name      account;
	  auto primary_key() const { return account.value; }
};
TABLE whitelist_table{
	  name      owner;
      string   wallet_address
	  
};
TABLE pause_table{
	  uint64_t            id;
	  bool                paused;
	  auto primary_key() const { return id; }
};
typedef eosio::multi_index< "accounts"_n, account > accounts;
typedef eosio::multi_index< "stat"_n, currency_stats > stats;
typedef eosio::multi_index< "blacklists"_n, blacklist_table > blacklists;
typedef eosio::multi_index< "pausetable"_n, pause_table > pausetable;
typedef eosio::multi_index< "whitelists"_n, whitelist_table > whitelists;
void sub_balance(name owner, asset value);
void add_balance(name owner, asset value, name ram_payer);
bool is_paused();
};

