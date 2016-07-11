/****************************************************************************/
/*! \file
 * \brief The Aggregate Data API
 *
 * Copyright (C) 2008-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 *
 * This API allows easy manipulation of aggregate data types such as
 * lists, hashes, ...
 */
/****************************************************************************/



/****************************************************************************/
/*! \defgroup types Data types
 * \brief AD API data type documentation.
 * @{ */

/*! \brief Master object type.
 *
 * Opaque data type used to reference master aggregate data object.
 */
typedef	void *ADOBJECT_t;

/*! \brief Element object type.
 *
 * Semi-opaque data type used to reference element data object.
 * Allows access to userdata.
 */
typedef struct ADnode_struct {
	void	*data;	/*!< pointer to userdata contained in node */
} *ADnode_t;

/*! \brief Node destructor callback type.
 *
 * Called by master object when it wants to delete a node.
 * Allows application to destroy userdata contained in the node.
 *
 * \param node	node containing userdata to destroy
 *
 * \pre node may be NULL
 * \pre node->data may be NULL
 *
 * \post no-op if 'node == NULL'
 * \post no-op if 'node->data == NULL'
 */
typedef void (*ADdestructor_t) (ADnode_t node);

/*! \brief Size measure type.
 *
 * Universal size data type.
 */
typedef unsigned long ADsize_t;

/*! \brief Offset measure type.
 *
 * Universal offset data type.
 *
 * \note Offset value may be negative to indicate index counting from
 * 	 end of list: -1 is last node, -2 is node before last, ...
 */
typedef signed long ADoffset_t;

/*! \brief Inspector callback type.
 *
 * Called by master object when walking through its list of nodes.
 * Allows access to userdata and can be used to request node deletion.
 *
 * \param node	node containing userdata to inspect
 * \return	node disposition request
 * \retval 0	request to keep node
 * \retval 1	request to delete node
 *
 * \todo Add support for callback to request abort of node walk operation...
 *
 * \pre node may be NULL
 * \pre node->data may be NULL
 *
 * \post return \c 0 if 'node == NULL'
 * \post return \c 0 if 'node->data == NULL'
 */
typedef int (*ADinspector_t) (ADnode_t node);

/*! \brief Iterator object type.
 *
 * Opaque data type used to reference node iterator object.
 */
typedef	void *ADiterator_t;

/*! \brief Comparator callback type.
 *
 * Called by master object when comparing nodes.
 * Allows application to compare userdata and help master object to
 * compare node content.
 *
 * \param node1	first node containing userdata to compare
 * \param node2 second node containing userdata to compare
 * \return	comparison result
 * \retval  0	node1 equals node2
 * \retval +1	node1 is greater than node2
 * \retval -1	node2 is greater than node1
 *
 * \pre node1 may be NULL
 * \pre node1->data may be NULL
 * \pre node2 may be NULL
 * \pre node2->data may be NULL
 *
 * \post return \c  0 if 'node1 == NULL' and 'node2 == NULL'
 * \post return \c -1 if 'node1 == NULL' and 'node2 != NULL'
 * \post return \c +1 if 'node1 != NULL' and 'node2 == NULL'
 * \post return \c  0 if 'node1->data == NULL' and 'node2->data == NULL'
 * \post return \c -1 if 'node1->data == NULL' and 'node2->data != NULL'
 * \post return \c +1 if 'node1->data != NULL' and 'node2->data == NULL'
 */
typedef	int (*ADcomparator_t) (ADnode_t node1, ADnode_t node2);

/*! \brief Duplicator callback type.
 *
 * Called by master object when it wants to copy a node.
 * Allows application to create a copy of the userdata.
 *
 * \param node	node containing userdata to copy
 * \return	pointer to copy of userdata
 *
 * \pre node \b must be non-NULL
 * \pre node->data \b must be non-NULL
 *
 * \post return \c NULL if 'node == NULL'
 * \post return \c NULL if 'node->data == NULL'
 * \post return \c NULL if out-of-memory (see also ::ADOOM_hook_t)
 */
typedef void * (*ADduplicator_t) (ADnode_t node);

/*! \brief Out of memory callback type.
 *
 * Called by master object when out of memory condition detected.
 * Allows application to handle OOM condition.
 *
 * \param function	name of function where OOM detected
 * \param file		source file name of caller
 * \param line		source file line number of caller
 * \param bytes		byte count of failed allocation request
 * \return		OOM condition disposition order
 * \retval 0	ignore failure and proceed with NULL value
 * \retval 1	retry allocation operation
 *
 * \pre file may be NULL
 * \pre function may be NULL
 * \pre line may be 0
 * \pre bytes may be 0
 *
 * \note This callback need not return to the caller. For example, the
 * 	 application may decide to abort() or terminate execution in some
 * 	 other way...
 */
typedef int (*ADOOM_hook_t) (char *file, char *function, unsigned long line, unsigned long bytes);

/*! @} */
/****************************************************************************/



/****************************************************************************/
/*! \defgroup API_object Basic object manipulation API.
 * \brief AD basic object function documentation.
 * @{ */


/*! \brief Create new object.
 *
 * \param destructor	destructor callback function
 * \param ...		extra object-specific creation attributes \n
 * 			(eg: comparator, allow-dups flag, ...)
 * \return		new object
 *
 * \pre destructor may be NULL
 *
 * \post returns \c NULL if invalid arguments
 * \post returns \c NULL if out of memory (see also ::ADOOM_hook_t)
 */
ADOBJECT_t ADOBJECT_create (ADdestructor_t destructor, ...);

/*! \brief Destroy object.
 *
 * Destroys object, nodes and userdata (using destructor callback) and sets object to NULL.
 *
 * \param object	pointer to object to destroy
 * \return		number of destroyed nodes
 *
 * \pre object may be NULL
 * \pre *object may be NULL
 *
 * \post returns \c 0 if 'object == NULL'
 * \post returns \c 0 if '*object == NULL'
 * \post calls \c abort() if object invalid
 */
ADsize_t ADOBJECT_destroy_safe (ADOBJECT_t *object);

/*! \brief Destroy object macro.
 *
 * Convenience macro for object destruction. Calls ADOBJECT_destroy_safe().
 */
#define	OBJECT_destroy(arg)	OBJECT_destroy_safe (&(arg))

/*! \brief Clear object.
 *
 * Clears all nodes from object (using destructor callback to destroy userdata).
 *
 * \param object	object to clear nodes from
 * \return		number of cleared nodes
 *
 * \pre object may be NULL
 *
 * \post returns \c 0 if 'object == NULL'
 * \post calls \c abort() if object invalid
 */
ADsize_t ADOBJECT_clear (ADOBJECT_t object);

/*! \brief Get object node count.
 *
 * Returns number of nodes contained in object.
 *
 * \param object	object to investigate
 * \return		number of nodes in object
 *
 * \pre object may be NULL
 *
 * \post returns \c 0 if 'object == NULL'
 * \post calls \c abort() if object invalid
 */
ADsize_t ADOBJECT_size	(ADOBJECT_t object);

/*! @} */
/****************************************************************************/



/****************************************************************************/
/*! \defgroup API_node_add Node addition API.
 * \brief AD node addition function documentation.
 * @{ */

/*! \brief Add node to object.
 *
 * Creates new node and adds it at default location.
 *
 * \param object	object to add new node to
 * \param data		userdata for new node
 * \return		new node
 *
 * \pre object may be NULL
 * \pre data may be NULL
 *
 * \post returns \c NULL if 'object == NULL'
 * \post calls \c abort() if object invalid
 * \post returns \c NULL if 'data == NULL'
 * \post returns \c NULL if duplicate node not allowed
 * \post returns \c NULL if out of memory (see also ::ADOOM_hook_t)
 */
ADnode_t ADOBJECT_add (ADOBJECT_t object, void *data);

/*! \brief Add node to object.
 *
 * Creates new node and adds it at specified offset.
 *
 * With a positive offset value it inserts the new node
 * right before the node specified by the offset.
 *
 * With a negative offset value it inserts the new node
 * right after the node specified by the offset.
 *
 * Example with object containsing nodes A, B and C:
 *
 * adding X at offset 0 = X, A, B, C = adding X at offset -4 \n
 * adding X at offset 1 = A, X, B, C = adding X at offset -3 \n
 * adding X at offset 2 = A, B, X, C = adding X at offset -2 \n
 * adding X at offset 3 = A, B, C, X = adding X at offset -1 \n
 *
 * \param object	object to add new node to
 * \param data		userdata for new node
 * \param offset	offset at wich to add new node
 * \return		new node
 *
 * \pre object may be NULL
 * \pre data may be NULL
 * \pre offset may be any value
 *
 * \post returns \c NULL if 'object == NULL'
 * \post calls \c abort() if object invalid
 * \post returns \c NULL if 'data == NULL'
 * \post returns \c NULL if 'offset > object.SIZE'
 * \post returns \c NULL if 'offset < -(object.SIZE + 1)'
 * \post returns \c NULL if duplicate node not allowed
 * \post returns \c NULL if out of memory (see also ::ADOOM_hook_t)
 *
 * \note object.SIZE refers to the count of nodes in object.
 */
ADnode_t ADOBJECT_add_idx (ADOBJECT_t object, void *data, ADoffset_t offset);

/*! \brief Add node to object.
 *
 * Creates new node and adds it at after specified node.
 *
 * \param node	node to add new node after
 * \param data	userdata for new node
 * \return	new node
 *
 * \pre node may be NULL
 * \pre data may be NULL
 *
 * \post returns \c NULL if 'node == NULL'
 * \post calls \c abort() if object invalid
 * \post returns \c NULL if 'data == NULL'
 * \post returns \c NULL if duplicate node not allowed
 * \post returns \c NULL if out of memory (see also ::ADOOM_hook_t)
 */
ADnode_t ADOBJECT_t_add_after (ADnode_t node, void *data);

/*! \brief Add node to object.
 *
 * Creates new node and adds it before after specified node.
 *
 * \param node	node to add new node in front of
 * \param data	userdata for new node
 * \return	new node
 *
 * \pre node may be NULL
 * \pre data may be NULL
 *
 * \post returns \c NULL if 'node == NULL'
 * \post calls \c abort() if object invalid
 * \post returns \c NULL if 'data == NULL'
 * \post returns \c NULL if duplicate node not allowed
 * \post returns \c NULL if out of memory (see also ::ADOOM_hook_t)
 */
ADnode_t ADOBJECT_t_add_before (ADnode_t node, void *data);

/*! \brief Add node to object.
 *
 * Creates new node and adds it at head position of object.
 *
 * \param object	object to add new node to
 * \param data		userdata for new node
 * \return		new node
 *
 * \pre object may be NULL
 * \pre data may be NULL
 *
 * \post returns \c NULL if 'object == NULL'
 * \post calls \c abort() if object invalid
 * \post returns \c NULL if 'data == NULL'
 * \post returns \c NULL if duplicate node not allowed
 * \post returns \c NULL if out of memory (see also ::ADOOM_hook_t)
 */
ADnode_t ADOBJECT_add_head (ADOBJECT_t object, void *data);

/*! \brief Add node to object.
 *
 * Creates new node and adds it at head position of object.
 *
 * \param object	object to add new node to
 * \param data		userdata for new node
 * \return		new node
 *
 * \pre object may be NULL
 * \pre data may be NULL
 *
 * \post returns \c NULL if 'object == NULL'
 * \post calls \c abort() if object invalid
 * \post returns \c NULL if 'data == NULL'
 * \post returns \c NULL if duplicate node not allowed
 * \post returns \c NULL if out of memory (see also ::ADOOM_hook_t)
 */
ADnode_t ADOBJECT_unshift (ADOBJECT_t object, void *data);

/*! \brief Add node to object.
 *
 * Creates new node and adds it at tail position of object.
 *
 * \param object	object to add new node to
 * \param data		userdata for new node
 * \return		new node
 *
 * \pre object may be NULL
 * \pre data may be NULL
 *
 * \post returns \c NULL if 'object == NULL'
 * \post calls \c abort() if object invalid
 * \post returns \c NULL if 'data == NULL'
 * \post returns \c NULL if duplicate node not allowed
 * \post returns \c NULL if out of memory (see also ::ADOOM_hook_t)
 */
ADnode_t ADOBJECT_add_tail (ADOBJECT_t object, void *data);

/*! \brief Add node to object.
 *
 * Creates new node and adds it at tail position of object.
 *
 * \param object	object to add new node to
 * \param data		userdata for new node
 * \return		new node
 *
 * \pre object may be NULL
 * \pre data may be NULL
 *
 * \post returns \c NULL if 'object == NULL'
 * \post calls \c abort() if object invalid
 * \post returns \c NULL if 'data == NULL'
 * \post returns \c NULL if duplicate node not allowed
 * \post returns \c NULL if out of memory (see also ::ADOOM_hook_t)
 */
ADnode_t ADOBJECT_push (ADOBJECT_t object, void *data);

/*! @} */
/****************************************************************************/



/****************************************************************************/
/*! \defgroup API_node_rem Node removal API.
 * \brief AD node removal function documentation.
 * @{ */

/*! \brief Remove node from object.
 *
 * Removes node from object, deletes it and returns userdata.
 *
 * \param node	node to remove
 * \return	userdata
 *
 * \pre node may be NULL
 *
 * \post returns \c NULL if 'node == NULL'
 * \post calls \c abort() if node invalid
 */
void *ADOBJECT_rem (ADnode_t node);

/*! \brief Remove node from object.
 *
 * Removes node at specified offset from object, deletes it and returns userdata.
 *
 * Example with object containing nodes A, B and C:
 *
 * removing node from offset 0 = B, C = removing node from offset -3 \n
 * removing node from offset 1 = A, C = removing node from offset -2 \n
 * removing node from offset 2 = A, B = removing node from offset -1 \n
 *
 * \param object	object to remove node from
 * \param offset	offset from wich to remove node
 * \return		userdata
 *
 * \pre object may be NULL
 * \pre offset may be any value
 *
 * \post returns \c NULL if 'object == NULL'
 * \post calls \c abort() if object invalid
 * \post returns \c NULL if 'offset > (object.SIZE - 1)'
 * \post returns \c NULL if 'offset < -object.SIZE'
 */
void *ADOBJECT_rem_idx (ADOBJECT_t object, ADoffset_t offset);

/*! \brief Remove node from object.
 *
 * Removes node after specified node, deletes it and returns userdata.
 *
 * \param node	node after which node should be removed
 * \return	userdata
 *
 * \pre node may be NULL
 *
 * \post returns \c NULL if 'node == NULL'
 * \post calls \c abort() if node invalid
 * \post returns \c NULL if no more nodes after node
 */
void *ADOBJECT_t_rem_after (ADnode_t node);

/*! \brief Remove node from object.
 *
 * Removes node before specified node, deletes it and returns userdata.
 *
 * \param node	node before which node should be removed
 * \return	userdata
 *
 * \pre node may be NULL
 *
 * \post returns \c NULL if 'node == NULL'
 * \post calls \c abort() if node invalid
 * \post returns \c NULL if no more nodes before node
 */
void *ADOBJECT_t_rem_before (ADnode_t node);

/*! \brief Remove node from object.
 *
 * Removes node at head position of object, deletes it and returns userdata.
 *
 * \param object	object to remove node from
 * \return		userdata
 *
 * \pre object may be NULL
 *
 * \post returns \c NULL if 'object == NULL'
 * \post calls \c abort() if object invalid
 * \post returns \c NULL if object contains no nodes
 */
void *ADOBJECT_rem_head (ADOBJECT_t object);

/*! \brief Remove node from object.
 *
 * Removes node at head position of object, deletes it and returns userdata.
 *
 * \param object	object to remove node from
 * \return		userdata
 *
 * \pre object may be NULL
 *
 * \post returns \c NULL if 'object == NULL'
 * \post calls \c abort() if object invalid
 * \post returns \c NULL if object contains no nodes
 */
void *ADOBJECT_shift (ADOBJECT_t object);

/*! \brief Remove node from object.
 *
 * Removes node at tail position of object, deletes it and returns userdata.
 *
 * \param object	object to remove node from
 * \return		userdata
 *
 * \pre object may be NULL
 *
 * \post returns \c NULL if 'object == NULL'
 * \post calls \c abort() if object invalid
 * \post returns \c NULL if object contains no nodes
 */
void *ADOBJECT_rem_tail (ADOBJECT_t object);

/*! \brief Remove node from object.
 *
 * Removes node at tail position of object, deletes it and returns userdata.
 *
 * \param object	object to remove node from
 * \return		userdata
 *
 * \pre object may be NULL
 *
 * \post returns \c NULL if 'object == NULL'
 * \post calls \c abort() if object invalid
 * \post returns \c NULL if object contains no nodes
 */
void *ADOBJECT_pop (ADOBJECT_t object);

/*! @} */
/****************************************************************************/



/****************************************************************************/
/*! \defgroup API_node_get Basic node access API.
 * \brief AD basic node access function documentation.
 * @{ */

/*! \brief Get access to node in object.
 *
 * Returns node at default location in object.
 *
 * \param object	object to return node from
 * \return		requested node
 *
 * \pre object may be NULL
 *
 * \post returns \c NULL if 'object == NULL'
 * \post calls \c abort() if object invalid
 * \post returns \c NULL if object contains no nodes
 */
ADnode_t ADOBJECT_get (ADOBJECT_t object);

/*! \brief Get access to node in object.
 *
 * Returns node at specified offset in object.
 *
 * Example with object containing nodes A, B and C:
 *
 * node at offset 0 = A = node at offset -3 \n
 * node at offset 1 = B = node at offset -2 \n
 * node at offset 2 = C = node at offset -1 \n
 *
 * \param object	object to return node from
 * \param offset	offset from wich to return node
 * \return		requested node
 *
 * \pre object may be NULL
 * \pre offset may be any value
 *
 * \post returns \c NULL if 'object == NULL'
 * \post calls \c abort() if object invalid
 * \post returns \c NULL if 'offset > (object.SIZE - 1)'
 * \post returns \c NULL if 'offset < -object.SIZE'
 */
ADnode_t ADOBJECT_get_idx (ADOBJECT_t object, ADoffset_t offset);

/*! \brief Get access to node in object.
 *
 * Returns node after specified node in object.
 *
 * \param node	node after which node should be returned
 * \return	requested node
 *
 * \pre node may be NULL
 *
 * \post returns \c NULL if 'node == NULL'
 * \post calls \c abort() if node invalid
 * \post returns \c NULL if no more nodes after node
 */
ADnode_t ADOBJECT_get_after (ADnode_t node);

/*! \brief Get access to node in object.
 *
 * Returns node after specified node in object.
 *
 * \param node	node after which node should be returned
 * \return	requested node
 *
 * \pre node may be NULL
 *
 * \post returns \c NULL if 'node == NULL'
 * \post calls \c abort() if node invalid
 * \post returns \c NULL if no more nodes after node
 */
ADnode_t ADOBJECT_next (ADnode_t node);

/*! \brief Get access to node in object.
 *
 * Returns node before specified node in object.
 *
 * \param node	node before which node should be returned
 * \return	requested node
 *
 * \pre node may be NULL
 *
 * \post returns \c NULL if 'node == NULL'
 * \post calls \c abort() if node invalid
 * \post returns \c NULL if no more nodes before node
 */
ADnode_t ADOBJECT_get_before (ADnode_t node);

/*! \brief Get access to node in object.
 *
 * Returns node before specified node in object.
 *
 * \param node	node before which node should be returned
 * \return	requested node
 *
 * \pre node may be NULL
 *
 * \post returns \c NULL if 'node == NULL'
 * \post calls \c abort() if node invalid
 * \post returns \c NULL if no more nodes before node
 */
ADnode_t ADOBJECT_prev (ADnode_t node);

/*! \brief Get access to node in object.
 *
 * Returns node at head position in object.
 *
 * \param object	object to return node from
 * \return		requested node
 *
 * \pre object may be NULL
 *
 * \post returns \c NULL if 'object == NULL'
 * \post calls \c abort() if object invalid
 * \post returns \c NULL if object contains no nodes
 */
ADnode_t ADOBJECT_get_head (ADOBJECT_t object);

/*! \brief Get access to node in object.
 *
 * Returns node at head position in object.
 *
 * \param object	object to return node from
 * \return		requested node
 *
 * \pre object may be NULL
 *
 * \post returns \c NULL if 'object == NULL'
 * \post calls \c abort() if object invalid
 * \post returns \c NULL if object contains no nodes
 */
ADnode_t ADOBJECT_peak_bottom (ADOBJECT_t object);

/*! \brief Get access to node in object.
 *
 * Returns node at tail position in object.
 *
 * \param object	object to return node from
 * \return		requested node
 *
 * \pre object may be NULL
 *
 * \post returns \c NULL if 'object == NULL'
 * \post calls \c abort() if object invalid
 * \post returns \c NULL if object contains no nodes
 */
ADnode_t ADOBJECT_get_tail (ADOBJECT_t object);

/*! \brief Get access to node in object.
 *
 * Returns node at tail position in object.
 *
 * \param object	object to return node from
 * \return		requested node
 *
 * \pre object may be NULL
 *
 * \post returns \c NULL if 'object == NULL'
 * \post calls \c abort() if object invalid
 * \post returns \c NULL if object contains no nodes
 */
ADnode_t ADOBJECT_peek_top (ADOBJECT_t object);

/*! @} */
/****************************************************************************/



/****************************************************************************/
/*! \defgroup API_node_wlk Advanced node access API.
 * \brief AD advanced node access function documentation.
 * @{ */

/*! \brief Access nodes sequentially.
 *
 * This function will walk sequentially through all nodes of an object,
 * starting with a specified node and walking in a specified direction,
 * calling the inspector callback function for each node.
 *
 * \param object	object to inspect
 * \param node		node from which to start inspection \n
 * 			or NULL to start from default start position
 * \param direction	direction in which to walk through object: \n
 * 			 0 = forwards \n
 * 			!0 = backwards
 * \param inspector	inspector callback function
 * \return		number of inspected nodes
 *
 * \pre object may be NULL
 * \pre node may be NULL
 * \pre inspector may be NULL
 *
 * \post returns \c 0 if 'object == NULL'
 * \post calls \c abort() if object invalid
 * \post calls \c abort() if node invalid
 * \post returns \c 0 if 'inspector == NULL'
 *
 * \note The default start node for forwards walking is the node
 * 	 at the head position of the object.
 *
 * \note The default start node for backwards walking is the node
 * 	 at the tail position of the object.
 */
ADsize_t ADOBJECT_walk (ADOBJECT_t object, ADnode_t node, int direction, ADinspector_t inspector);

/*! \brief Create new object iterator.
 *
 * This function creates a new iterator which can be used to walk sequentially
 * through all nodes of an object, between specified start and end nodes and
 * in a specified direction.
 *
 * \param object	object to inspect
 * \param start		node from which to start iterator \n
 * 			or NULL to start from default start position
 * \param stop		node at which to stop iterator \n
 * 			or NULL to stop at default start position
 * \param direction	direction in which to walk through object: \n
 * 			 0 = forwards \n
 * 			!0 = backwards
 *
 * \pre object may be NULL
 * \pre start may be NULL
 * \pre stop may be NULL
 *
 * \post returns \c NULL if 'object == NULL'
 * \post calls \c abort() if object invalid
 * \post calls \c abort() if start node invalid
 * \post calls \c abort() if stop node invalid
 * \post returns \c NULL if out of memory (see also ::ADOOM_hook_t)
 *
 * \note the default start and stop nodes for a forward iterator are
 * 	 the nodes at the head and the tail position of the object.
 *
 * \note the default start and stop nodes for a backward iterator are
 * 	 the nodes at the tail and the head position of the object. (note the reversed positions)
 *
 * \note When the start node comes after the stop node in the object, the new iterator
 * 	 will be equivalent to an iterator with switched start and stop nodes and reversed
 * 	 direction. \n
 * 	 \n
 * 	 For example:\n
 * 	 	from C to A, direction 0   ==>   from A to C, direction 1 \n
 * 	 	from C to A, direction 1   ==>   from A to C, direction 0
 */
ADiterator_t ADOBJECT_iter_new (ADOBJECT_t object, ADnode_t start, ADnode_t stop, int direction);

/*! \brief Destroy iterator.
 *
 * \param iterator	iterator to destroy
 *
 * \pre iterator may be NULL
 *
 * \post no-op if 'iterator == NULL'
 */
void ADOBJECT_iter_del (ADiterator_t iterator);

/*! \brief Access next node via iterator.
 *
 * \param iterator	iterator to use
 * \return		requested node
 *
 * \pre iterator may be NULL
 *
 * \post returns \c NULL if 'iterator == NULL'
 * \post returns \c NULL if no more nodes available
 */
ADnode_t ADOBJECT_iterate (ADiterator_t iterator);

/*! \example iterator_access_pattern.c
 * The iterator has been created to allow this access pattern:
 */

/*! \brief Reset iterator.
 *
 * Rewinds iterator to start node.
 *
 * \param iterator	iterator to rewind
 *
 * \pre iterator may be NULL
 *
 * \post no-op if 'iterator == NULL'
 * \post no-op if no more nodes available
 */
void ADOBJECT_iter_rst (ADiterator_t iterator);

/*! @} */
/****************************************************************************/



/****************************************************************************/
/*! \defgroup API_node Basic node manipulation API.
 * \brief AD basic node manipulation function documentation.
 * @{ */

/*! \brief Update node userdata.
 *
 * Updates userdata in node, destroys old userdata using destructor callback.
 *
 * \param node	node to update
 * \param data	new userdata
 * \return	updated node
 *
 * \pre node may be NULL
 * \pre data may be NULL
 *
 * \post returns \c NULL if 'node == NULL'
 * \post calls \c abort() if node invalid
 * \post returns \c NULL if 'data == NULL'
 */
ADnode_t ADOBJECT_update (ADnode_t node, void *data);

/*! \brief Query node index.
 *
 * Returns index of first occurrence of node in object.
 *
 * \param node	node to query index of
 * \return	offset of node in object
 *
 * \pre node may be NULL
 *
 * \post returns \c 0 if 'node == NULL'
 * \post calls \c abort() if node invalid
 */
ADoffset_t ADOBJECT_indexof (ADnode_t node);

/*! \brief Query node index.
 *
 * Returns index of first occurrence of node in object.
 *
 * \param node	node to query index of
 * \return	offset of node in object
 *
 * \pre node may be NULL
 *
 * \post returns \c 0 if 'node == NULL'
 * \post calls \c abort() if node invalid
 */
ADoffset_t ADOBJECT_indexof_first (ADnode_t node);

/*! \brief Query node index.
 *
 * Returns index of last occurrence of node in object.
 *
 * \param node	node to query index of
 * \return	offset of node in object
 *
 * \pre node may be NULL
 *
 * \post returns \c 0 if 'node == NULL'
 * \post calls \c abort() if node invalid
 */
ADoffset_t ADOBJECT_indexof_last (ADnode_t node);

/*! \brief Find node in object.
 *
 * Finds node in object containing specified userdata.
 *
 * \param object	object in which to look for node
 * \param data		userdata to look for
 * \param comparator	comparator callback function
 * \return		node with userdata
 *
 * \pre object may be NULL
 * \pre data may be NULL
 * \pre comparator may be NULL
 *
 * \post returns \c NULL if 'object == NULL'
 * \post calls \c abort() if object invalid
 * \post returns \c NULL if 'data == NULL'
 * \post returns \c NULL if 'comparator == NULL'
 * \post returns \c NULL if node not found
 */
ADnode_t ADOBJECT_find (ADOBJECT_t object, void *data, ADcomparator_t comparator);

/*! @} */
/****************************************************************************/



/****************************************************************************/
/*! \defgroup API_object_adv Advanced object manipulation API.
 * \brief AD advanced object manipulation function documentation.
 * @{ */

/*! \brief Create copy of object.
 *
 * Creates deep copy of object, using duplicator callback to copy userdata.
 *
 * \param object	object to copy
 * \param duplicator	duplicator callback function
 * \return		new object
 *
 * \pre object may be NULL
 * \pre duplicator may be NULL
 *
 * \post returns \c NULL if 'object == NULL'
 * \post calls \c abort() if object invalid
 * \post returns \c NULL if 'duplicator == NULL'
 * \post returns \c NULL if out of memory (see also ::ADOOM_hook_t)
 */
ADOBJECT_t ADOBJECT_copy (ADOBJECT_t object, ADduplicator_t duplicator);

/*! \brief Create copy of object.
 *
 * Creates shallow copy of object and does not duplicate userdata.
 *
 * \param object	object to copy
 * \return		new object
 *
 * \pre object may be NULL
 *
 * \post returns \c NULL if 'object == NULL'
 * \post calls \c abort() if object invalid
 * \post returns \c NULL if out of memory (see also ::ADOOM_hook_t)
 */
ADOBJECT_t ADOBJECT_clone (ADOBJECT_t object);

/*! \brief Split object in two.
 *
 * Splits object in two right before specified node. Creates new object
 * to hold second part of original object.
 *
 * \param object	object to split
 * \param node		node right before which object should be split
 * \return		new object containing second part of original object
 *
 * \pre object may be NULL
 * \pre node may be NULL
 *
 * \post returns \c NULL if 'object == NULL'
 * \post calls \c abort() if object invalid
 * \post returns \c NULL if 'node == NULL'
 * \post calls \c abort() if node invalid
 * \post returns \c NULL if out of memory (see also ::ADOOM_hook_t)
 */
ADOBJECT_t ADOBJECT_split (ADOBJECT_t object, ADnode_t node);

/*! \brief Merge two objects.
 *
 * Merges two objects into one. After the merge the second object is destroyed,
 * unless duplicate nodes are not allowed in the first object. In that case
 * the second object will remain containing any duplicate nodes (if any).
 *
 * \param object1	first object
 * \param object2	pointer to second object
 * \return		number of merge nodes (new nodes in object1 after merge)
 *
 * \pre object1 may be NULL
 * \pre object2 may be NULL
 * \pre *object2 may be NULL
 *
 * \post returns \c 0 if 'object1 == NULL' and does not modify object2
 * \post calls \c abort() if object1 invalid
 * \post returns \c 0 if 'object2 == NULL' and does not modify object1
 * \post returns \c 0 if '*object2 == NULL' and does not modify object1
 * \post calls \c abort() if *object2 invalid
 * \post '*object2 == NULL' if duplicate nodes allowed for object1
 * \post object2 contains duplicate nodes (if any) if duplicate nodes not allowed for object1
 * \post returns \c NULL if out of memory (see also ::ADOOM_hook_t)
 */
ADsize_t ADOBJECT_merge_safe (ADOBJECT_t object1, ADOBJECT_t *object2);

/*! \brief Merge objects macro.
 *
 * Convenience macro for object merge. Calls ADOBJECT_merge_safe().
 */
#define	ADOBJECT_merge(A,B)	ADOBJECT_merge_safe (A, &(B))

/*! \brief Sort nodes in object.
 *
 * \param object	object to sort
 * \param comparator	comparator callback function
 * \return		number of sorted nodes
 *
 * \pre object may be NULL
 * \pre comparator may be NULL
 *
 * \post returns \c 0 if 'object == NULL'
 * \post calls \c abort() if object invalid
 * \post returns \c 0 if 'comparator == NULL'
 */
ADsize_t ADOBJECT_sort (ADObject_t object, ADcomparator_t comparator);

/*! @} */
/****************************************************************************/



/****************************************************************************/
/*! \defgroup API_EH Error handling API.
 * \brief AD error handling function documentation.
 * @{ */

/*! \brief Install global OOM callback.
 *
 * This functions installs a callback that will be used by all objects.
 *
 * \param OOM	OOM callback to install \n
 * 		or NULL to remove installed callback
 * \return	previous value of OOM callback
 *
 * \pre OOM may be NULL
 *
 * \post returns \c NULL if no previous callback
 */
ADOOM_hook_t AD_set_OOM (ADOOM_hook_t OOM);

/*! \brief Install object-local OOM callback.
 *
 * This function installs a callback for the specified object. It overrides
 * the global OOM callback for this object only.
 *
 * \param object	object for which to install the OOM callback
 * \param OOM		OOM callback to install \n
 * 			or NULL to remove installed callback
 * \return		old value of OOM callback
 *
 * \pre object may be NULL
 * \pre OOM may be NULL
 *
 * \post returns \c NULL if 'object == NULL'
 * \post returns \c NULL if no previous callback
 *
 */
ADOOM_hook_t ADOBJECT_set_OOM (ADOBJECT_t object, ADOOM_hook_t OOM);

/*! @} */
/****************************************************************************/



/****************************************************************************/
/*! \defgroup Future Future features.
 * \brief Possible future API features.
 * @{ */

typedef struct ADfreezer_struct {
	ADsize_t	size;
	void		*data;
} *ADfreezer_t;

typedef ADfreezer_t (*ADfreezer) (ADnode_t node);

ADfreezer_t	ADOBJECT_freeze (ADOBJECT_t object);

ADOBJECT_t	ADOBJECT_thaw	(ADfreezer_t freezer);

/*! @} */
/****************************************************************************/

