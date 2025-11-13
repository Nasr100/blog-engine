 // Dark mode functionality
        const themeToggle = document.querySelector('.theme-toggle');
        const currentTheme = localStorage.getItem('theme');

        // Check for saved theme or system preference
        if (currentTheme === 'dark' || (currentTheme === null && window.matchMedia('(prefers-color-scheme: dark)').matches)) {
                document.documentElement.setAttribute('data-theme', 'dark');
        }

        // Toggle theme on button click
        themeToggle.addEventListener('click', () => {
        const isDark = document.documentElement.getAttribute('data-theme') === 'dark';
        
        if (isDark) {
            document.documentElement.removeAttribute('data-theme');
            localStorage.setItem('theme', 'light');
        } else {
            document.documentElement.setAttribute('data-theme', 'dark');
            localStorage.setItem('theme', 'dark');
        }
        });