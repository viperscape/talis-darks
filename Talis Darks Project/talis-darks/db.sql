drop table if exists turns;
drop table if exists games;
drop table if exists players;

create table if not exists turns (
    games_id integer not null, 
    player_id integer not null,
    turn text not null,
    tstamp datetime default current_timestamp
);

create table if not exists games (
    player_id integer not null,
    player_id2 integer,
    tstamp datetime default current_timestamp
);

insert into games (player_id) values (1);

create table if not exists players (
    rand_id char(32) not null,
    create_tstamp datetime default current_timestamp,
    login_tstamp datetime default current_timestamp
);

insert into players (rand_id) values ((hex(randomblob(16))));