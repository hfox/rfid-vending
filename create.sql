\connect mydb

CREATE SCHEMA rustvaluta ;

CREATE TABLE rustvaluta.accounts (
	id SERIAL PRIMARY KEY NOT NULL,
	name VARCHAR(100) NOT NULL UNIQUE,
	card VARCHAR(100) NOT NULL UNIQUE,
	saldo FLOAT NOT NULL DEFAULT 0
);

CREATE INDEX by_name ON rustvaluta.accounts ( name );
CREATE INDEX by_card ON rustvaluta.accounts ( card );

CREATE TABLE rustvaluta.transactions (
	id SERIAL PRIMARY KEY NOT NULL,
	account INT NOT NULL,
	change FLOAT NOT NULL,
	timestamp timestamp NOT NULL
);

CREATE INDEX by_account ON rustvaluta.transactions ( account );
CREATE INDEX by_timestamp ON rustvaluta.transactions ( timestamp );

