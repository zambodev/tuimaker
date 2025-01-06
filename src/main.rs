use tuimaker::window_system::window_manager::WindowManager;
use tuimaker::window_system::window::Window;


fn main() {
    let term_size = termsize::get().unwrap();

    let mut wm = WindowManager::new();

    let w1 = Window::new(0, 0, term_size.cols.into(), term_size.rows.into(), true);
    w1.lock().unwrap().draw();
    wm.add_root(w1.clone());

    let w2 = Window::new(10, 10, 60, 10, false);
    w2.lock().unwrap().draw();
    w1.lock().unwrap().add_child(w2.clone()); 

    wm.render(w1.clone());

    loop {}
}