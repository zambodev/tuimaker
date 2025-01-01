use tuimaker::window_system::window::*;
use tuimaker::window_system::window_manager::*;


fn main() {
    let term_size = termsize::get().unwrap();
    let window = Window::new(0, 0, term_size.cols.into(), term_size.rows.into());

    let mut wm = WindowManager::new(&window);

    window.print();
}