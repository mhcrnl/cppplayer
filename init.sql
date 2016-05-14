CREATE TABLE IF NOT EXISTS artists (
	id INTEGER PRIMARY KEY,
	name TEXT
);

CREATE TABLE IF NOT EXISTS songs (
	spath TEXT PRIMARY KEY,
	title TEXT,
	artist TEXT,
	FOREIGN KEY(artist) REFERENCES artists(id)
);

CREATE INDEX index_song_title ON songs(spath);
