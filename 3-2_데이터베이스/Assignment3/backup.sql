create view all_customer as
select branch_name,customer_name 
from (customer natural join borrower) natural join loan_branch;


select customer_name 
from all_customer
where branch_name='숭실대지점';


create view only_account_customer as
select distinct customer_name, customer_city
from customer natural join depositor
where customer_id not in (select customer_id from customer natural join borrower);


select customer_name
from only_account_customer
where customer_city='서울';


select customer_name
from only_account_customer
where customer_city='서울';


select customer_id, avg_balance
from customer_avg_balance
where customer_name='홍길동';


DELIMITER $$
create function get_loan_counts(customer_id varchar(10)) returns integer
begin
	declare lCount integer;
		select count(*) into lCount
		from customer natural join borrower
		where customer.customer_id = customer_id;
		return lCount;
end $$ DELIMITER;


select distinct customer_name, customer_city
from customer natural join borrower
where get_loan_counts(customer_id) >1 ;