

typedef struct avltreenode_t   avltreenode_t;

struct avltreenode_t {
  void          *key;         /* key for this node */
  void          *value;       /* value stored at this node */
  avltreenode_t *parent;
  avltreenode_t *left;        /* left subtree */
  avltreenode_t *right;       /* right subtree */
  unsigned int   height;     /* height (right) - height (left) */
};

/**
 * Get avl node height.
 */
avltreenode_t* avltreenode_get_height(const avltreenode_t* root)
{
    return root == NULL ? -1 : (int)root->height;
}

/**
 * ll Rotate.
 */
avltreenode_t* avltreenode_left_signal_rotate(avltreenode_t* root) {
    /*
     *     A         B
     *    /         / \
     *   B    =>   C   A
     *  / \           /
     * C   D         D
     */
    avltreenode_t* left = NULL;
    if (root == NULL || root->left == NULL) {
        return root;
    }
    
    /* rotate */
    left = root->left;
    root->left = left->right;
    left->right = root;
    /* change parent */
    root->parent = left;
    if (root->left != NULL) {
        root->left->parent = root;
    }
    /* update height */
    root->height = (unsigned int)(
        (avltreenode_get_height(root->left) > avltreenode_get_height(root->right) ?
         avltreenode_get_height(root->left) : avltreenode_get_height(root->right)) + 1);
    left->height = (unsigned int)(
        (avltreenode_get_height(left->left) > avltreenode_get_height(pleft->right) ?
         avltreenode_get_height(left->left) : avltreenode_get_height(left->right)) + 1);

    return left;
}

/**
 * rr Rotate.
 */
avltreenode_t* avltreenode_right_signal_rotate(avltreenode_t* root)
{
    /*
     *  A              B
     *   \            / \
     *    B     =>   A   D
     *   / \          \
     *  C   D          C
     */
    avltreenode_t* right = NULL;
    if (root == NULL || root->right == NULL) {
        return root;
    }

    /* rotate */
    right = root->right;
    root->right = right->left;
    right->left = root;
    /* change parent */
    root->parent = right;
    if (root->right != NULL) {
        root->right->parent = root;
    }
    /* update height */
    root->height = (unsigned int)(
        (avltreenode_get_height(root->left) > avltreenode_get_height(root->right) ?
         avltreenode_get_height(root->left) : avltreenode_get_height(root->right)) + 1);
    right->height = (unsigned int)(
        (avltreenode_get_height(right->left) > avltreenode_get_height(right->right) ?
         avltreenode_get_height(right->left) : avltreenode_get_height(right->right)) + 1);

    return right;
}

/**
 * lr Rotate.
 */
avltreenode_t* avltreenode_left_double_rotate(avltreenode_t* root)
{
    /*
     *       A               A                 E
     *      / \             / \              /   \
     *     B   C           E   C            B     A
     *    / \       =>    / \        =>    / \   / \
     *   D   E           B   G            D   F G   C
     *      / \         / \
     *     F   G       D   F
     */        
    if(root == NULL) {
        return root;
    }

    root->left = avltreenode_right_signal_rotate(root->left);
    root->left->parent = root;
    return avltreenode_left_signal_rotate(root);
}

/**
 * rl Rotate.
 */
avltreenode_t* avltreenode_right_double_rotate(avltreenode_t* root)
{
    /*
     *       A               A                   D 
     *      / \             / \                /   \
     *     B   C           B   D              A     C
     *        / \    =>       / \       =>   / \   / \
     *       D   E           F   C          B   F G   E
     *      / \                 / \
     *     F   G               G   E
     */
    if(root == NULL) {
        return root;
    }

    root->right = avltreenode_left_signal_rotate(root->right);
    root->right->parent = root;
    return avltreenode_right_signal_rotate(root);
}


/**
 * Rebalance the subtree and update the root height.
 */
avltreenode_t* avltreenode_dobalance(avltreenode_t* root)
{
    if (root != NULL) {
        /* if left - right > 1 */
        if (avltreenode_get_height(root->left) - avltreenode_get_height(root->right) > 1) {
            /* ll */
            if (avltreenode_get_height(root->left->left) >=
                avltreenode_get_height(root->left->right)) {
                root = avltreenode_left_signal_rotate(root);
            } else {
            /* lr */
                root = avltreenode_left_double_rotate(root);
            }
        } else if (avltreenode_get_height(root->right) - avltreenode_get_height(root->left) > 1) {
        /* else if right - left > 1 */
            /* rr */
            if (avltreenode_get_height(root->right->right) >= 
                avltreenode_get_height(root->right->left)) {
                root = avltreenode_right_signal_rotate(root);
            } else {
            /* rl */
                root = avltreenode_right_double_rotate(root);
            }
        }

        root->height = (unsigned int)(
            (avltreenode_get_height(root->left) > avltreenode_get_height(root->right) ?
             avltreenode_get_height(root->left) : avltreenode_get_height(root->right)) + 1);
    }

    return root;
}


/**
 * GTree:
 *
 * The GTree struct is an opaque data structure representing a
 * [balanced binary tree][glib-Balanced-Binary-Trees]. It should be
 * accessed only by using the following functions.
 */
struct avltree_t {
  avltreenode_t        *root;
  GCompareDataFunc  key_compare;
  GDestroyNotify    key_destroy_func;
  GDestroyNotify    value_destroy_func;
  gpointer          key_compare_data;
  guint             nnodes;
  gint              ref_count;
};




