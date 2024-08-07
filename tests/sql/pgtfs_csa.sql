-- SPDX-FileCopyrightText: © 2024 Adrian C. Prelipcean
--
-- SPDX-License-Identifier: EUPL-1.2

create extension pgtfs;

-- simple connectivity test
select
    stop_id,
    stop_sequence,
    arrival_time,
    trip_id
from pgtfs_csa(
    '1', '2', 1712707200.0,
    $$
select trip_id, stop_id, arrival_time::double precision, departure_time::double precision, stop_sequence::int from (
    values
     ('1753','1',NULL,1712757660,1),
     ('1753','2',1712759880,1712759940,2),
     ('1753','3',1712762040,1712762100,3),
     ('1753','4',1712762100,NULL,4)
    )
    as data (trip_id, stop_id, arrival_time, departure_time, stop_sequence);
$$
);

-- simple connectivity test where sequence has a gap (3 is missing)
select
    stop_id,
    stop_sequence,
    arrival_time,
    trip_id
from pgtfs_csa(
    '1', '4', 1712707200.0,
    $$
select trip_id, stop_id, arrival_time::double precision, departure_time::double precision, stop_sequence::int from (
    values
     ('1753','1',NULL,1712757660,1),
     ('1753','2',1712759880,1712759940,2),
     ('1753','3',1712762040,1712762100,4),
     ('1753','4',1712762100,NULL,5)
    )
    as data (trip_id, stop_id, arrival_time, departure_time, stop_sequence);
$$
);

-- connected and route on single trip
select
    stop_id,
    stop_sequence,
    arrival_time,
    trip_id
from pgtfs_csa(
    '10017', '51607', 1712707200.0,
    $$
select trip_id, stop_id, arrival_time::double precision, departure_time::double precision, stop_sequence::int from (
    VALUES
     ('2077','12948',NULL,1712782320,1),
     ('2077','22151',1712782590,1712782620,2),
     ('2077','22163',1712782890,1712782920,3),
     ('2077','22175',1712783220,1712783700,4),
     ('2077','22216',1712784420,1712784480,5),
     ('2077','22230',1712784840,1712784900,6),
     ('2077','22254',1712785320,1712785440,7),
     ('2077','22278',1712785950,1712785980,8),
     ('2077','22280',1712786280,1712786760,9),
     ('2077','22292',1712787090,1712787120,10),
     ('2077','22307',1712787300,1712787360,11),
     ('2077','22319',1712787630,1712787660,12),
     ('2077','22321',1712787900,1712787960,13),
     ('2077','22345',1712788350,1712788380,14),
     ('2077','22357',1712788620,1712788680,15),
     ('2077','22369',1712789010,1712789040,16),
     ('2077','22371',1712789700,1712789760,17),
     ('2077','22395',1712789940,1712790000,18),
     ('2077','22400',1712790360,1712790420,19),
     ('2077','22412',1712790570,1712790600,20),
     ('2077','22424',1712790870,1712790900,21),
     ('2077','22436',1712791020,NULL,22),
     ('3535','30691',NULL,1712779200,1),
     ('3535','30835',1712779860,1712779920,2),
     ('3535','30859',1712780640,1712780700,3),
     ('3535','30885',1712781600,1712781660,4),
     ('3535','30897',1712781990,1712782020,5),
     ('3535','30902',1712782470,1712782500,6),
     ('3535','30914',1712782830,1712782860,7),
     ('3535','30938',1712783460,1712783520,8),
     ('3535','30952',1712784090,1712784120,9),
     ('3535','30988',1712784720,1712784780,10),
     ('3535','31011',1712785740,1712785800,11),
     ('3535','31023',1712786190,1712786220,12),
     ('3535','31047',1712787180,1712787240,13),
     ('3535','31073',1712788020,1712788080,14),
     ('3535','31097',1712788530,1712788560,15),
     ('3535','31102',1712789160,1712789220,16),
     ('3535','31126',1712789970,1712790000,17),
     ('3535','31140',1712790780,1712790840,18),
     ('3535','57132',1712791230,1712791260,19),
     ('3535','31164',1712791620,1712791680,20),
     ('3535','31190',1712792220,1712792280,21),
     ('3535','31217',1712793000,1712793300,22),
     ('3535','31243',1712793840,1712793900,23),
     ('3535','31255',1712794230,1712794260,24),
     ('3535','31279',1712794560,1712794620,25),
     ('3535','31293',1712795010,1712795040,26),
     ('3535','31308',1712795370,1712795400,27),
     ('3535','31334',1712795760,NULL,28),
     ('1753','10017',NULL,1712757660,1),
     ('1753','50017',1712759880,1712759940,2),
     ('1753','50213',1712762040,1712762100,3),
     ('1753','50342',1712763960,1712764080,4),
     ('1753','50433',1712765820,1712765880,5),
     ('1753','50603',1712768160,1712768220,6),
     ('1753','50691',1712769360,1712769420,7),
     ('1753','50811',1712770860,1712770920,8),
     ('1753','51059',1712773080,1712773560,9),
     ('1753','51190',1712775300,1712775360,10),
     ('1753','51358',1712777040,1712777100,11),
     ('1753','51451',1712778000,1712778060,12),
     ('1753','51530',1712779080,1712779140,13),
     ('1753','51607',1712779800,1712779920,14),
     ('1753','51645',1712780220,NULL,15)
    )
    as data (trip_id, stop_id, arrival_time, departure_time, stop_sequence);
$$
);

-- connected and route on multiple trips
select
    stop_id,
    stop_sequence,
    arrival_time,
    trip_id
from pgtfs_csa(
    '10017', '51607', 1712707200.0,
    $$
select trip_id, stop_id, arrival_time::double precision, departure_time::double precision, stop_sequence::int from (
    VALUES
     ('1663','10017',NULL,1712750400,1),
     ('1663','50017',1712752620,1712752740,2),
     ('1663','50213',1712754840,1712754960,3),
     ('1663','50342',1712756820,1712756940,4),
     ('1663','50433',1712758680,1712758800,5),
     ('1663','50603',1712761080,1712761200,6),
     ('1663','60309',1712763660,1712764560,7),
     ('1663','60452',1712766180,1712766540,8),
     ('1663','60490',1712767320,1712767440,9),
     ('1663','60593',1712769120,1712769180,10),
     ('1663','60658',1712769960,1712770080,11),
     ('1663','60725',1712771160,1712771220,12),
     ('1663','60804',1712772720,1712772780,13),
     ('1663','60878',1712773680,1712773740,14),
     ('1663','60921',1712773980,NULL,15),
     ('5105','50342',NULL,1712757900,1),
     ('5105','50392',1712758740,1712758800,2),
     ('5105','50419',1712759280,1712759340,3),
     ('5105','50433',1712759940,1712760000,4),
     ('5105','50471',1712760450,1712760480,5),
     ('5105','50495',1712761020,1712761080,6),
     ('5105','50512',1712761560,1712761620,7),
     ('5105','50548',1712762160,1712762220,8),
     ('5105','50603',1712762820,1712762940,9),
     ('5105','50653',1712763600,1712763660,10),
     ('5105','50691',1712764320,1712764380,11),
     ('5105','50732',1712764920,1712764980,12),
     ('5105','50744',1712765190,1712765220,13),
     ('5105','50768',1712765580,1712765640,14),
     ('5105','50770',1712765850,1712765880,15),
     ('5105','50811',1712766420,NULL,16),
     ('1753','10017',NULL,1712757660,1),
     ('1753','50017',1712759880,1712759940,2),
     ('1753','50213',1712762040,1712762100,3),
     ('1753','50342',1712763960,1712764080,4),
     ('1753','50433',1712765820,1712765880,5),
     ('1753','50603',1712768160,1712768220,6),
     ('1753','50691',1712769360,1712769420,7),
     ('1753','50811',1712770860,1712770920,8),
     ('1753','51059',1712773080,1712773560,9),
     ('1753','51190',1712775300,1712775360,10),
     ('1753','51358',1712777040,1712777100,11),
     ('1753','51451',1712778000,1712778060,12),
     ('1753','51530',1712779080,1712779140,13),
     ('1753','51607',1712779800,1712779920,14),
     ('1753','51645',1712780220,NULL,15)
    )
    as data (trip_id, stop_id, arrival_time, departure_time, stop_sequence);
$$
);

-- disconnected (origin and destination exist)
select
    stop_id,
    stop_sequence,
    arrival_time,
    trip_id
from pgtfs_csa(
    '1', '8', 1712707200.0,
    $$
select trip_id, stop_id, arrival_time::double precision, departure_time::double precision, stop_sequence::int from (
    values
     ('1753','1',NULL,1712757660,1),
     ('1753','2',1712759880,1712759940,2),
     ('1753','3',1712762040,1712762100,3),
     ('1753','4',1712762100,NULL,4),
     ('1754','5',NULL,1712757660,1),
     ('1754','6',1712759880,1712759940,2),
     ('1754','7',1712762040,1712762100,3),
     ('1754','8',1712762100,NULL,4)
    )
    as data (trip_id, stop_id, arrival_time, departure_time, stop_sequence);
$$
);

-- origin does not exist in the network 
select
    stop_id,
    stop_sequence,
    arrival_time,
    trip_id
from pgtfs_csa(
    '8', '2', 1712707200.0,
    $$
select trip_id, stop_id, arrival_time::double precision, departure_time::double precision, stop_sequence::int from (
    values
     ('1753','1',NULL,1712757660,1),
     ('1753','2',1712759880,1712759940,2),
     ('1753','3',1712762040,1712762100,3),
     ('1753','4',1712762100,NULL,4)
    )
    as data (trip_id, stop_id, arrival_time, departure_time, stop_sequence);
$$
);

-- destination does not exist in the network
select
    stop_id,
    stop_sequence,
    arrival_time,
    trip_id
from pgtfs_csa(
    '1', '8', 1712707200.0,
    $$
select trip_id, stop_id, arrival_time::double precision, departure_time::double precision, stop_sequence::int from (
    values
     ('1753','1',NULL,1712757660,1),
     ('1753','2',1712759880,1712759940,2),
     ('1753','3',1712762040,1712762100,3),
     ('1753','4',1712762100,NULL,4)
    )
    as data (trip_id, stop_id, arrival_time, departure_time, stop_sequence);
$$
);

-- origin and destination do not exist in the network
select
    stop_id,
    stop_sequence,
    arrival_time,
    trip_id
from pgtfs_csa(
    '7', '8', 1712707200.0,
    $$
select trip_id, stop_id, arrival_time::double precision, departure_time::double precision, stop_sequence::int from (
    values
     ('1753','1',NULL,1712757660,1),
     ('1753','2',1712759880,1712759940,2),
     ('1753','3',1712762040,1712762100,3),
     ('1753','4',1712762100,NULL,4)
    )
    as data (trip_id, stop_id, arrival_time, departure_time, stop_sequence);
$$
);

-- null cases
select
    stop_id,
    stop_sequence,
    arrival_time,
    trip_id
from pgtfs_csa(
    NULL, '2', 1712707200.0,
    $$
select trip_id, stop_id, arrival_time::double precision, departure_time::double precision, stop_sequence::int from (
    values
     ('1753','1',NULL,1712757660,1),
     ('1753','2',1712759880,1712759940,2),
     ('1753','3',1712762040,1712762100,3),
     ('1753','4',1712762100,NULL,4)
    )
    as data (trip_id, stop_id, arrival_time, departure_time, stop_sequence);
$$
);

select
    stop_id,
    stop_sequence,
    arrival_time,
    trip_id
from pgtfs_csa(
    '1', NULL, 1712707200.0,
    $$
select trip_id, stop_id, arrival_time::double precision, departure_time::double precision, stop_sequence::int from (
    values
     ('1753','1',NULL,1712757660,1),
     ('1753','2',1712759880,1712759940,2),
     ('1753','3',1712762040,1712762100,3),
     ('1753','4',1712762100,NULL,4)
    )
    as data (trip_id, stop_id, arrival_time, departure_time, stop_sequence);
$$
);

select
    stop_id,
    stop_sequence,
    arrival_time,
    trip_id
from pgtfs_csa(
    '1', '2', NULL,
    $$
select trip_id, stop_id, arrival_time::double precision, departure_time::double precision, stop_sequence::int from (
    values
     ('1753','1',NULL,1712757660,1),
     ('1753','2',1712759880,1712759940,2),
     ('1753','3',1712762040,1712762100,3),
     ('1753','4',1712762100,NULL,4)
    )
    as data (trip_id, stop_id, arrival_time, departure_time, stop_sequence);
$$
);

select
    stop_id,
    stop_sequence,
    arrival_time,
    trip_id
from pgtfs_csa('10017', '51607', 1712707200.0, NULL);

-- negative reference time case
select
    stop_id,
    stop_sequence,
    arrival_time,
    trip_id
from pgtfs_csa(
    '1', '2', -1,
    $$
select trip_id, stop_id, arrival_time::double precision, departure_time::double precision, stop_sequence::int from (
    values
     ('1753','1',NULL,1712757660,1),
     ('1753','2',1712759880,1712759940,2),
     ('1753','3',1712762040,1712762100,3),
     ('1753','4',1712762100,NULL,4)
    )
    as data (trip_id, stop_id, arrival_time, departure_time, stop_sequence);
$$
);

-- check that same route is prioritized when minimize_transfers is TRUE

select
    stop_id,
    stop_sequence,
    arrival_time,
    trip_id
from pgtfs_csa(
    '1', '4', 1712707200.0,
    $$
select trip_id, stop_id, arrival_time::double precision, departure_time::double precision, stop_sequence::int from (
    values
     ('1753','1',NULL,1712757660,1),
     ('1753','2',1712759880,1712759940,2),
     ('1753','3',1712762040,1712762100,3),
     ('1753','4',1712762100,NULL,4),
     ('1754','1',NULL,1712757660,1),
     ('1754','2',1712759880,1712759940,2),
     ('1754','4',1712762100,NULL,4)
    )
    as data (trip_id, stop_id, arrival_time, departure_time, stop_sequence);
$$,
    TRUE
);

-- check that same route is not prioritized when minimize_transfers is FALSE

select
    stop_id,
    stop_sequence,
    arrival_time,
    trip_id
from pgtfs_csa(
    '1', '4', 1712707200.0,
    $$
select trip_id, stop_id, arrival_time::double precision, departure_time::double precision, stop_sequence::int from (
    values
     ('1753','1',NULL,1712757660,1),
     ('1753','2',1712759880,1712759940,2),
     ('1753','3',1712762040,1712762100,3),
     ('1753','4',1712762100,NULL,4),
     ('1754','1',NULL,1712757660,1),
     ('1754','2',1712759880,1712759940,2),
     ('1754','4',1712762100,NULL,4)
    )
    as data (trip_id, stop_id, arrival_time, departure_time, stop_sequence);
$$,
    FALSE
);
