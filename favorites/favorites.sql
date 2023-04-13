-- update a single element in the shows column
UPDATE shows SET title = "How I Met Your Mother" WHERE title = "How i met your mother";

-- '%ame%' means select any title that has 'ame' in it. e.g G'ame' of Thrones
SELECT * FROM shows WHERE title LIKE '%ame%';

-- 'A%e' means starts with an A and ends in an e
SELECT * FROM shows WHERE title LIKE 'A%e';

UPDATE shows SET title = 'Adventure Time' WHERE title = 'adventure time';

-- to distinguish an apostrophe in a string from the apostrophes surrounding the string we use ''
UPDATE shows SET title = 'It''s Always Sunny in Philidelphia' WHERE title = 'Its Always Sunny in Philidelphia';
