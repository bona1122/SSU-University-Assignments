alter table customer add customer_type varchar(10);


select customer_name
from customer
where customer_city = '서울';


select distinct customer_name
from ((customer natural join borrower) natural join loan_branch )natural join branch
where customer_city = branch_city;


select sum(amount)
from (customer natural join borrower) natural join loan
where customer_name like '김%';


select branch_name
from branch
where assets > (select avg(assets) from branch);


select distinct customer_id,customer_name 
from customer natural join depositor
where customer_id not in (select customer_id from customer natural join borrower);


select customer_id, avg(balance)
from (customer natural join depositor) natural join account
where customer_city = '서울' and
customer_id in (select customer_id from (customer natural join depositor) group by customer_id having count(*)>2)
group by customer_id;


select distinct b1.branch_name
from branch as b1, branch as b2
where b1.assets>b2.assets and
b1.branch_city='부산';


with inSeoul(num) as
	(select count(*)
	from branch
	where branch_city = '서울' ),
joined(branch_name, customer_id,branch_city) as
	(select distinct branch_name,customer_id,branch_city
	from ((borrower natural join loan_branch) natural join customer) natural join branch )


select c1.customer_id, c1.customer_name
from customer as c1, inseoul
where inSeoul.num = (
		select count(*)
		from Joined as j
		where c1.customer_id = j.customer_id and branch_city = '서울');



update account 
set balance = case
when balance >= 1000 then balance*1.05 else balance*1.03 end;


update customer
set customer_type = 'VIP'
where customer_id in (
		select c2.customer_id
		from(
			select c1.customer_id
			from customer as c1, depositor, account
			where c1.customer_id = depositor.customer_id and account.account_number = depositor.account_number
			group by c1.customer_id
			having sum(balance) > 10000
		) as c2
);