# asset2tokenz

This contract is to allow users to tokenize assets for Non-Fungible Tokens(NFT),
Fungible Tokens(FT),and Non-Transferable Tokens(NTTs). User can request and receive validations of their assets,
receive tokens with relative parameters and request confirmations, including

Contract ACTIONS

 regauthor		(name author, data, stemplate, string imgpriority)  
 authorupdate		(author, data, stemplate, string imgpriority)  


 # -- For Non-Fungible Tokens (NFTs)---
 
 create			(author, category, owner, idata, mdata, requireсlaim)  
 update			(author, owner, assetid, mdata)  
 transfer		(from, to , [assetid1,..,assetidn], memo)  
 burn			  (owner, [assetid1,..,assetidn], memo)  
 
 offer			(owner, newowner, [assetid1,..,assetidn], memo)  
 canceloffer		(owner, [assetid1,..,assetidn])  
 claim			(claimer, [assetid1,..,assetidn])  
  
 delegate		(owner, to, [assetid1,..,assetidn], period, memo)  
 undelegate		(owner, from, [assetid1,..,assetidn])  
 delegatemore		(owner, assetid, period)  
 
 attach			(owner, assetidc, [assetid1,..,assetidn])
 detach			(owner, assetidc, [assetid1,..,assetidn])
 
 attachf		(owner, author, quantity, assetidc)
 detachf		(owner, author, quantity, assetidc)
 
 
 # -- For Fungible Tokens (FTs) ---
 
 createf		(author, maximum_supply, authorctrl, data)
 updatef		(author, sym, data)
 issuef			(to, author, quantity, memo)
 transferf		(from, to, author, quantity, memo)
 burnf			(from, author, quantity, memo)

 offerf			(owner, newowner, author, quantity, memo)
 cancelofferf		(owner, [ftofferid1,...,ftofferidn])
 claimf			(claimer, [ftofferid1,...,ftofferidn])

 openf			(owner, author, symbol, ram_payer)
 closef			(owner, author, symbol)
 
 
 # -- For Non-Transferable Tokens (NTTs) ---

 createntt	(author, category, owner, idata, mdata, requireсlaim)  
 updatentt	(author, owner, assetid, mdata)  
 burnntt		(owner, [assetid1,..,assetidn], memo)  
 claimntt		(claimer, [assetid1,..,assetidn])  
 
 
- Assets

sasset {  
	uint64_t	id; 		// asset id used for transfer and search;  
	name		owner;  	// asset owner (mutable - by owner!!!);  
	name		author;		// asset author (game contract, immutable);  
	name		category;	// asset category, chosen by author, immutable;  
	string		idata;		// immutable assets data. Can be stringified JSON or just sha256 string;  
	string		mdata;		// mutable assets data, added on creation or asset update by author. Can be  
					// stringified JSON or just sha256 string;  
	sasset[]	container;	// other NFTs attached to this asset
	account[]	containerf;	// FTs attached to this asset 



- Perform read/write/remove operations on the table
