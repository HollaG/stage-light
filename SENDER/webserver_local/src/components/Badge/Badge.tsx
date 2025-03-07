import classes from "./Badge.module.css"

const Badge = ({ children, color, ...props }) => {
  return (
    <span className={`${classes.badge} ${classes[color]}`} {...props}>
      {children}
    </span>
  );
}

export default Badge;