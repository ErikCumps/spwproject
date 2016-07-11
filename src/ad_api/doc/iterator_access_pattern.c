ADOBJECT_t	object;
ADnode_t	from, to, node;
int		dir;
ADiterator_t	iter;

iter = ADOBJECT_iter_new (object, from, to, dir); 
while (node = ADOBJECT_iterate (iter)) {
	/* access node via 'node' variable */
	some_function (node);
}
