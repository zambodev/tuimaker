use crate::window_system::window::Window;

struct WindowNode<'a> {
    window: Option<&'a Window>,
    children: Vec<WindowNode<'a>>
}

impl<'a> WindowNode<'a> {
    fn new(win: &'a Window) -> Self {
        WindowNode {
            window: Some(win),
            children: Vec::new()
        }
    }

    fn add_child(&mut self, child: &'a Window) {
        self.children.push(WindowNode::new(child));
    }
}

struct WindowTree<'b> {
    head: Vec<WindowNode<'b>>
}

impl<'b> WindowTree<'b> {
    fn new(win: &'b Window) -> Self {
        WindowTree {
            head: vec![WindowNode::new(win)]
        }
    }

    fn insert(&mut self, win: &'b Window) {
        self.head.push(WindowNode::new(win));
    }
}


pub struct WindowManager<'c> {
    tree: WindowTree<'c>
}

impl<'c> WindowManager<'c> {
    pub fn new(win: &'c Window) -> Self {
        WindowManager {
            tree: WindowTree::new(win)
        }
    }
}