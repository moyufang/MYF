```scss
// 间距工具类生成
$spacings: (0, 4, 8, 12, 16, 24, 32, 48, 64);
$directions: (t: top, r: right, b: bottom, l: left, x: horizontal, y: vertical);

@each $size in $spacings {
  $i: index($spacings, $size) - 1;
  
  // 边距
  @each $prefix, $dir in $directions {
    .m#{$prefix}-#{$i} {
      @if $prefix == 'x' {
        margin-left: #{$size}px;
        margin-right: #{$size}px;
      } @else if $prefix == 'y' {
        margin-top: #{$size}px;
        margin-bottom: #{$size}px;
      } @else if $prefix == 'a' {
        margin: #{$size}px;
      } @else {
        margin-#{$dir}: #{$size}px;
      }
    }
    
    // 内边距（类似实现）
  }
}

// 响应式变体
@each $breakpoint, $width in (sm:576, md:768, lg:992, xl:1200) {
  @media (min-width: #{$width}px) {
    @each $size in $spacings {
      $i: index($spacings, $size) - 1;
      .#{$breakpoint}\:m-#{$i} {
        margin: #{$size}px;
      }
      // 其他响应式类...
    }
  }
}
```

```



```