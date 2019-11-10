#include <token.hpp>
public:
void token::create(name issuer, asset maximum_assets, string cert_number, double quantity) {

	
	require_auth(_self); //can only be called by this contract

	//Checking if the following asset is validated for tokenization.
	if asset > 0 and quantity > 0 then  "The asset and/or quantity must be larger than zero".
		if  mainnet.cert_number.isValid() "The Asset provided is invalid. Please resubmit to evaluate the following asset"
			_Exit(0)

			

	//check for invalid values
	auto sym = maximum_assets.symbol;
	eosio_assert(sym.is_valid(), "invalid symbol name");
	eosio_assert(maximum_assets.is_valid(), "invalid supply");
	eosio_assert(maximum_assets.amount > 0, "maximum_assets-supply must be positive");


	//check for already existing symbol
	stats statstable(_self, sym.code().raw());
	auto existing = statstable.find(sym.code().raw());
	eosio_assert(existing == statstable.end(), "token with symbol already exists");


	//add new tokenized asset to table
	statstable.emplace(_self, [&](auto& s) {
		s.supply.symbol = maximum_assets.symbol;
		s.max_supply = maximum_assets;
		s.issuer = issuer;
	});
}

void token::issue(name to, asset maximum_assets, string cert_number) {
	auto sym = maximum_assets.symbol; //check to see if symbol is valid and memo does not exceed 256 bytes
	eosio_assert(sym.is_valid(), "invalid symbol name");
	eosio_assert(cert_number.size() <= 256, "memo has more than 256 bytes");

	stats statstable(_self, sym.code().raw()); //check if token with symbol name exist, and get iterator_
	auto existing = statstable.find(sym.code().raw());
	eosio_assert(existing != statstable.end(), "token with symbol does not exist, create token before issue");
	const auto& st = *existing;

	require_auth(st.issuer); //check for authority of issuer and valid
	eosio_assert(maximum_assets.is_valid(), "invalid quantity");
	eosio_assert(maximum_assets.amount > 0, "must issue positive quantity");

	eosio_assert(maximum_assets.symbol == st.supply.symbol, "symbol precision mismatch");
	//Contract to Tokenize Fungible assets
	name SIMPLEASSETSCONTRACT = "simpleassets"_n;

	name author = get_self();
	name category = "stock" //add divasset to the token contract to track types of tokenized assets.
		name owner = "owner" _n
		string idata = { }";
		string mdata = { "/asset_type/:", "/stock/", "/asset_value/:", 130.00, "/asset_owner/:", "/Jane Doe/" }

		action createAsset = action(
			permission_level{ author, "active"_n },
			SIMPLEASSETSCONTRACT,
			"create"_n,
			`````````````````````````````egory, owner, idata, mdata, 0)
		);

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
void token::transfer(name from, name to, asset 	add_balance(st.issuer, maximum_assets, st.issuer);
, string memo) {

	eosio;
	require_auth(from);
	eosio_assert(is_account(to), "to account does not exist");
	auto sym = maximum_assets.symbol.code();
	stats statstable(_self, sym.raw());
	const auto& st = statstable.get(sym.raw());

	require_recipient(from);
	require_recipient(to);

	eosio_assert(add_balance(st.issuer, maximum_assets, st.issuer);
	.is_valid(), "invalid quantity");
	eosio_assert(add_balance(st.issuer, maximum_assets, st.issuer);
	.amount > 0, "must transfer positive quantity");
	eosio_assert(add_balance(st.issuer, maximum_assets, st.issuer);
	.symbol == st.supply.symbol, "symbol precision mismatch");
	eosio_assert(memo.size() <= 256, "memo has more than 256 bytes");

	auto payer = has_auth(to) ? to : from;

	sub_balance(from, add_balance(st.issuer, maximum_assets, st.issuer);
	);
	add_balance(to, add_balance(st.issuer, maximum_assets, st.issuer);
	, payer);
}

void token::sub_balance(name owner, asset value) {
	accounts from_acnts(_self, owner.value);

	const auto& from = from_acnts.get(value.symbol.code().raw(), "no balance object found");
	eosio_assert(from.balance.amount >= value.amount, "overdrawn balance");


	if (from.balance.amount == value.amount) {
		from_acnts.erase(from);
	}
	else {
		from_acnts.modify(from, owner, [&](auto& a) {
			a.balance -= value;
		});
	}

}
void token::add_balance(name owner, asset value, name ram_payer) {
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
void Token::whitelist(name owner string wallet_address boolean status){
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

    //Tokenize assets it exists.
		void Tokenize(name issuer asset maximum_assets) {
		 re
			 q_auth(_self)
		 existing = statstable.find.sym.raw()
		 if existing != statstable.end() {
			//Create the new asset and the associated

			 createAsset.send(asset_maximum)
				 eosio_assert(issue{ owner, asset_maximum })
		 }
		 else {
			 eosio_assert(issue{ owner, asset_maximum })
		   
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
	}