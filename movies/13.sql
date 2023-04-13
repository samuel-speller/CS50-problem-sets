SELECT distinct(name)
FROM people
JOIN stars ON people.id = stars.person_id
WHERE people.name != 'Kevin Bacon'
AND stars.movie_id IN
(SELECT movie_id FROM stars
JOIN people ON people.id = stars.person_id
WHERE people.name = 'Kevin Bacon' AND people.birth = 1958);